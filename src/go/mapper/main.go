package main

import (
	"C"
	"fmt"
	"log"
	"runtime"
	"time"

	"github.com/go-gl/gl/v3.2-core/gl"
	"github.com/go-gl/glfw/v3.2/glfw"
	"github.com/golang-ui/nuklear/nk"
	"github.com/sqweek/dialog"
	"github.com/xlab/closer"
	
)

const (
	winWidth  = 1024
	winHeight =  800

	maxVertexBuffer  = 512 * 1024
	maxElementBuffer = 128 * 1024
)

func init() {
	runtime.LockOSThread()
}

func main() {
	if err := glfw.Init(); err != nil {
		closer.Fatalln(err)
	}
	glfw.WindowHint(glfw.ContextVersionMajor, 3)
	glfw.WindowHint(glfw.ContextVersionMinor, 2)
	glfw.WindowHint(glfw.OpenGLProfile, glfw.OpenGLCoreProfile)
	glfw.WindowHint(glfw.OpenGLForwardCompatible, glfw.True)
	win, err := glfw.CreateWindow(winWidth, winHeight, "Portal Editor", nil, nil)
	if err != nil {
		closer.Fatalln(err)
	}
	win.MakeContextCurrent()

	width, height := win.GetSize()
	log.Printf("glfw: created window %dx%d", width, height)

	if err := gl.Init(); err != nil {
		closer.Fatalln("opengl: init failed:", err)
	}
	gl.Viewport(0, 0, int32(width), int32(height))

	ctx := nk.NkPlatformInit(win, nk.PlatformInstallCallbacks)

	atlas := nk.NewFontAtlas()
	nk.NkFontStashBegin(&atlas)
	// sansFont := nk.NkFontAtlasAddFromBytes(atlas, MustAsset("assets/FreeSans.ttf"), 16, nil)
	// config := nk.NkFontConfig(14)
	// config.SetOversample(1, 1)
	// config.SetRange(nk.NkFontChineseGlyphRanges())
	// simsunFont := nk.NkFontAtlasAddFromFile(atlas, "/Library/Fonts/Microsoft/SimHei.ttf", 14, &config)
	nk.NkFontStashEnd()
	// if simsunFont != nil {
	// 	nk.NkStyleSetFont(ctx, simsunFont.Handle())
	// }

	exitC := make(chan struct{}, 1)
	doneC := make(chan struct{}, 1)
	closer.Bind(func() {
		close(exitC)
		<-doneC
	})

	state := &State{
		bgColor: nk.NkRgba(28, 48, 62, 255),
		editMode: MapMode,
		curMap: freshMap(),
		curSector: nil,
		curWall: nil,
		curVertex: nil,
	}

	fpsTicker := time.NewTicker(time.Second / 30)
	for {
		select {
		case <-exitC:
			nk.NkPlatformShutdown()
			glfw.Terminate()
			fpsTicker.Stop()
			close(doneC)
			return
		case <-fpsTicker.C:
			if win.ShouldClose() {
				close(exitC)
				continue
			}
			glfw.PollEvents()
			gfxMain(win, ctx, state)
		}
	}
}

type Opt struct {
	label string
	val Option
	get func() Option
	set func(Option)
}

type Button struct {
	label string
	action func(state* State) 
}


func gfxMain(win *glfw.Window, ctx *nk.Context, state *State) {
	nk.NkPlatformNewFrame()

	
	// Layout
	//drawToolsWindow(win, ctx, state)
	
	bounds := nk.NkRect(25, 25, 250, 500)
	update := nk.NkBegin(ctx, "Map Tools", bounds,
		nk.WindowBorder|nk.WindowMovable|nk.WindowScalable|nk.WindowMinimizable|nk.WindowTitle)
	
	if update > 0 {
		switch state.editMode {
		case VertexMode:
			drawVertexMode(win, ctx, state)
		case LineMode:
			drawLineMode(win, ctx, state)
		case SectorMode:
			drawSectorMode(win, ctx, state)
		case MapMode:
			drawMapMode(win, ctx, state)
		}
	}
	
	nk.NkEnd(ctx)

	// Render
	bg := make([]float32, 4)
	nk.NkColorFv(bg, state.bgColor)
	width, height := win.GetSize()
	gl.Viewport(0, 0, int32(width), int32(height))
	gl.Clear(gl.COLOR_BUFFER_BIT)
	gl.ClearColor(bg[0], bg[1], bg[2], bg[3])
	nk.NkPlatformRender(nk.AntiAliasingOn, maxVertexBuffer, maxElementBuffer)
	win.SwapBuffers()
}


func drawOptionsRow(win *glfw.Window, ctx *nk.Context, state *State, cols int32, options []Opt) {
	
	nk.NkLayoutRowDynamic(ctx, 30, cols)
	{
		for _,opt := range options {
			if nk.NkOptionLabel(ctx, opt.label, flag(opt.get() == opt.val)) > 0 {
				opt.set(opt.val)
			}
		}
	}
}


func drawModeOptions(win *glfw.Window, ctx *nk.Context, state *State) {
	getMode := func() Option { return state.editMode }
	setMode := func(o Option) { state.editMode = o }
	modeOptions := []Opt{
		{"vertex",VertexMode,getMode,setMode},{"line",LineMode,getMode,setMode},
		{"sector",SectorMode,getMode,setMode},{"map", MapMode,getMode,setMode},
	}
	drawOptionsRow(win, ctx, state, 2, modeOptions)
}


func drawButtonRow(win *glfw.Window, ctx *nk.Context, state *State, cols int32, btns []Button) {
	nk.NkLayoutRowDynamic(ctx, 30, cols) // 30,80,2
	{
		
		for _,btn := range btns {
			if nk.NkButtonLabel(ctx, btn.label) > 0 {
				btn.action(state)
			}
		}
	}
}


func drawVertexMode(win *glfw.Window, ctx *nk.Context, state *State) {
	drawModeOptions(win, ctx, state)
}

func drawLineMode(win *glfw.Window, ctx *nk.Context, state *State) {
	drawModeOptions(win, ctx, state)
}

func drawSectorListItem(win *glfw.Window, ctx *nk.Context, state *State, i int, sector Sector) {
	
	nk.NkLayoutRowBegin(ctx, nk.Dynamic, 30, 3)
	nk.NkLayoutRowPush(ctx, float32(1)/8)
	nk.NkLabel(ctx, fmt.Sprintf("%d: ", i), nk.TextLeft)
	
	nk.NkLayoutRowPush(ctx, float32(4)/8)
	nk.NkLabel(ctx, fmt.Sprintf("f: %d, c: %d", sector.floorHeight, sector.ceilHeight), nk.TextLeft)

	nk.NkLayoutRowPush(ctx, float32(3)/8)
	if nk.NkButtonLabel(ctx, "Select") == 1 {
		//state.curSector = state.curMap.sectors[sector.index]
	}
	nk.NkLayoutRowEnd(ctx)
}


func drawSectorList(win *glfw.Window, ctx *nk.Context, state *State) {
	nk.NkLayoutRowDynamic(ctx, 250, 1)
	{
		opts := nk.Flags(nk.WindowBorder)
		if nk.NkGroupBeginTitled(ctx, "Sectors", "Sectors", opts) != 0 {
			
			{
				
				for i,sector:= range state.curMap.sectors {
					drawSectorListItem(win,ctx,state,i,sector)
				}
				
				
				
				
			}
			nk.NkGroupEnd(ctx)
		}
	}
	
}

func drawSector(win *glfw.Window, ctx *nk.Context, state *State, sector *Sector) {
	nk.NkLayoutRowDynamic(ctx, 20, 1)
	nk.NkLabel(ctx, fmt.Sprintf("Cur sector: %d", sector.index), nk.TextLeft)
}

func drawSectorMode(win *glfw.Window, ctx *nk.Context, state *State) {
	drawModeOptions(win, ctx, state)

	if nk.NkButtonLabel(ctx, "New Sector") > 0 {
		state.curSector = addNewSector(&state.curMap)
		log.Printf("num sectors %d", len(state.curMap.sectors))
	}
		

	if state.curSector != nil {
		drawSector(win, ctx, state, state.curSector)
	}

	drawSectorList(win, ctx, state)
}

func drawMapMode(win *glfw.Window, ctx *nk.Context, state *State) {
	drawModeOptions(win, ctx, state)
	mapButtons := []Button{
		{"New", newMap}, {"Load", loadMap},
		{"Save", saveMap}, {"Export", exportMap},
		{"Generate PVS", genPVS},
	}
	drawButtonRow(win, ctx, state, 2, mapButtons)

}


func drawToolsWindow(win *glfw.Window, ctx *nk.Context, state *State) {
	//drawModeOptions(win, ctx, state)
	
	
	nk.NkLayoutRowDynamic(ctx, 20, 1)
	{
		nk.NkLabel(ctx, "background:", nk.TextLeft)
	}
	nk.NkLayoutRowDynamic(ctx, 25, 1)
	{
		size := nk.NkVec2(nk.NkWidgetWidth(ctx), 400)
		if nk.NkComboBeginColor(ctx, state.bgColor, size) > 0 {
			nk.NkLayoutRowDynamic(ctx, 120, 1)
			cf := nk.NkColorCf(state.bgColor)
			cf = nk.NkColorPicker(ctx, cf, nk.ColorFormatRGBA)
			state.bgColor = nk.NkRgbCf(cf)
			nk.NkLayoutRowDynamic(ctx, 25, 1)
			r, g, b, a := state.bgColor.RGBAi()
			r = nk.NkPropertyi(ctx, "#R:", 0, r, 255, 1, 1)
			g = nk.NkPropertyi(ctx, "#G:", 0, g, 255, 1, 1)
			b = nk.NkPropertyi(ctx, "#B:", 0, b, 255, 1, 1)
			a = nk.NkPropertyi(ctx, "#A:", 0, a, 255, 1, 1)
			state.bgColor.SetRGBAi(r, g, b, a)
			nk.NkComboEnd(ctx)
		}
	}
}

type Option uint8

const (
	VertexMode Option = 0
	LineMode   Option = 1
	SectorMode Option = 2
	MapMode    Option = 3
)

type State struct {
	bgColor nk.Color
	prop    int32
	editMode Option

	curMap  Map
	curSector *Sector
	curWall *Wall
	curVertex *Vertex
}

func onError(code int32, msg string) {
	log.Printf("[glfw ERR]: error %d: %s", code, msg)
}



func loadMap(state *State) {
	log.Printf("loading map")
	filename,_ := dialog.File().Title("Load map").Load()
	log.Printf("load from %s", filename)
}

func saveMap(state *State) {
	log.Printf("saving map")
	filename,_ := dialog.File().Title("Save map").Save()
	log.Printf("saving to %s", filename)
}

func newMap(state *State) {
	log.Printf("creating new map")
	ok := dialog.Message("%s", "Do you want to reset the map data?").Title("Are you sure?").YesNo()
	if ok {
		state.curMap = freshMap()
	}
}

func exportMap(state *State) {
	log.Printf("exporting map")
	filename,_ := dialog.File().Title("Export map").Save()
	log.Printf("exporting to %s", filename)
}

func genPVS(state *State) {
	log.Printf("generating pvs for map")
}
