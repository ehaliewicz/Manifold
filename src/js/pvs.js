

(function() {
    

    const canvas = document.getElementById("render-canvas");
    const ctx = canvas.getContext("2d");
    

    // a sector is a start wall vertex index, plus the number of walls
    const sectors = [
		[ 0, 8, 0], // sector 0
		[ 8, 6, 1], // sector 1
		[14, 6, 2], // sector 2
		[20, 5, 3], // sector 3
		[25, 4, 4], // sector 4
		[29, 4, 5], // sector 5
		[33, 6, 6], // sector 6
		[39, 5, 7], // sector 7
		[44, 4, 8], // sector 8
		[48, 4, 9],  // sector 9
		[52, 4, 10], // sector 10
		[56, 4, 11], // sector 11
    ];


    // a vertex is just two numbers
    const vertexes = [
		// sector 0 walls
		[5,2],
		[7,0],
		[13,0],
		[14,3],
		[14,6],
		[12,8],
		[8,8],
		[5,5],
		// sector 1 walls
		[0,5],
		[0,4],
		[2,2],
		[5,2],
		[5,5],
		[3,5],
		// sector 2 walls
		[0,5],
		[3,5],
		[4,6],
		[4,7],
		[4,10],
		[0,5.5],
		// sector 3 walls
		[4,7],
		[6,9],
		[6,11],
		[5,11],
		[4,10],
		// sector 4 walls
		[6,9],
		[9,9],
		[9,11],
		[6,11],
		// sector 5 walls
		[14,3],
		[17,3],
		[17,6],
		[14,6],
		// sector 6 walls
		[17,-1],
		[19,-1],
		[19,1],
		[19,4],
		[17,6],
		[17,3],
		// sector 7 walls
		[6,11],
		[9,11],
		[9,15],
		[6,15],
		[6,13],
		// sector 8 walls
		[6,13],
		[2,13],
		[2,11],
		[6,11],
		// sector 9 walls
		[6,13],
		[6,15],
		[2,16],
		[2,14],
		// sector 10 walls
		[19,-1],
		[23,-1],
		[23, 1],
		[19, 1],
		// sector 11 walls
		[14, 6],
		[16, 8],
		[14,10],
		[12, 8]

	
		
    ];

    const portals = [
		// sector 0 portals
		-1,
		-1,
		-1,
		5,
		11,
		-1,
		-1,
		1,
		// sector 1 portals
		-1,
		-1,
		-1,
		0,
		-1,
		2,
		// sector 2 portals
		1,
		-1,
		-1,
		3,
		-1,
		-1,
		// sector 3 portals
		-1,
		4,
		-1,
		-1,
		2,
		// sector 4 portals
		-1,
		-1,
		7,
		3,
		// sector 5 portals
		-1,
		6,
		-1,
		0,
		// sector 6 portals
		-1,
		10,
		-1,
		-1,
		5,
		-1,
		// sector 7 portals
		4,
		-1,
		-1,
		9,
		8,
		// sector 8 portals
		-1,
		-1,
		-1,
		7,
		// sector 9 portals
		7,
		-1,
		-1,
		-1,
		// sector 10 portals
		-1,
		-1,
		-1,
		6,
		// sector 11 portals
		-1,
		-1,
		-1,
		0

    ];
    

    // gather set of walls
    // [v1, v2, portal_tgt]
    const get_walls_for_sector = function(sector) {
		const [v_idx, num_walls, sect_num] = sector;
		const res = [];
		for(var i = 0; i < num_walls-1; i++) {
			res.push([vertexes[v_idx+i], vertexes[v_idx+i+1], portals[v_idx+i]]);
		}
		res.push([vertexes[v_idx+num_walls-1], vertexes[v_idx], portals[v_idx+num_walls-1]]);
		
		return res;
    }

    const get_vertexes_for_sector = function(sector) {
		const [v_idx, num_walls, sect_num] = sector;
		const res = [];
		for(var i = 0; i < num_walls; i++) {
			res.push(vertexes[v_idx+i]);
		}
		return res;
		}
		
	const xOff = 5;
	const yOff = 5;


	const trans_coords = function(v) {
		const [x,y] = v;
		
		
		return [(x+xOff)*20, (y+yOff)*20];
    }

    const untrans_coord = function(v) {
		const [x,y] = v

		const utx = (x/20)-xOff;
		const uty = (y/20)-yOff;
		return [utx, uty];
    }

    const get_wall_normal = function(wall) {
		const [v1x,v1y] = wall[0];
		const [v2x,v2y] = wall[1];
		const dx = v2x - v1x;
		const dy = v2y - v1y;
		return [-dy,dx];
    }

    const is_wall_backfacing = function(src_wall, dst_wall) {
		const [nx1,ny1] = get_wall_normal(src_wall);
		const [nx2,ny2] = get_wall_normal(dst_wall);
		console.log("checking: " + src_wall + " and " + dst_wall)
		const res = (nx1*nx2+ny1*ny2);
		console.log("res: ", res);
		return res < 0;
		
	
    }

    const draw_wall = function(wall) {
		const [v1, v2, portal_tgt] = wall;
		if(portal_tgt === -1) {
			ctx.strokeStyle = '#000000';
		} else if (portal_tgt === -2) {
			ctx.strokeStyle = '#0000FF';
		} else if (portal_tgt === -3) {
			ctx.strokeStyle = '#00FF00';
		} else {
			ctx.strokeStyle = '#FF0000';
		}
		ctx.beginPath();

		
		
		const [v1x,v1y] = trans_coords(v1);
		const [v2x,v2y] = trans_coords(v2);
		
		ctx.moveTo(v1x, v1y);
		ctx.lineTo(v2x, v2y);
		
		ctx.stroke();

    }

    const draw_polygon = function(verts) {
		//const trans_verts = verts.map(function([v1,v2]) { return [trans_coords(v1), trans_coords(v2)]});
		const trans_verts = verts.map(trans_coords);
		
		//const tv1 = trans_coords(v1);
		//const tv2 = trans_coords(v2);
		//const tv3 = trans_coords(v3);
		ctx.fillStyle = '#0000FF22';
		ctx.beginPath();
		ctx.moveTo(...trans_verts[0]);

		for(var i = 1; i < verts.length; i++) {
			ctx.lineTo(...trans_verts[i]);
		}
		ctx.lineTo(...trans_verts[0]);

		ctx.fill();
	
    }

    const get_center_of_sector = function(sector) {
		let x = 0;
		let y = 0;
		const [v_idx,num_walls, sect_num] = sector;

		for(var i = 0; i < num_walls; i++) {
			const [cx,cy] = vertexes[i+v_idx];
			x += cx;
			y += cy;
		}

		x /= num_walls;
		y /= num_walls;
		return [x,y];
    }

    const get_level_bounds = function(vertexes) {
	let minX = vertexes[0][0];
	let maxX = minX;
	let minY = vertexes[0][1];
	let maxY = minY;
	
	vertexes.forEach(function ([x,y]) {
	    minX = Math.min(x, minX);
	    maxX = Math.max(x, maxX);
	    minY = Math.min(y, minY);
	    maxY = Math.max(y, maxY);
	});
	return [[minX, minY], [maxX, maxY]];
    }

    const draw_sector = function(sector) {
		const sect_num = sector[2];
		const walls = get_walls_for_sector(sector);
		
		
		walls.forEach(draw_wall);
		
		
		ctx.font = '17px sans-serif';
		const center = get_center_of_sector(sector);
		
		const [tcx,tcy] = trans_coords(center);
		ctx.fillStyle = '#000000';
		ctx.fillText(''+sect_num, (tcx-5), (tcy+5)); 
		
    }

    const draw_sectors = function() {
		sectors.forEach(draw_sector);
    }

    
    const draw_map = function() {
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		
		draw_sectors();

		const [[minX,minY],[maxX,maxY]] = get_level_bounds(vertexes);
		draw_wall([[minX, minY], [maxX,minY], -1]);
		draw_wall([[minX, maxY], [maxX,maxY], -1]);
		draw_wall([[minX, minY], [minX,maxY], -1]);
		draw_wall([[maxX, minY], [maxX,maxY], -1]);
		}
		

		const display_pvs = function(pvs) {
		document.getElementById("cur-pvs").innerText = "cur pvs: " + Array.from(pvs.values());
	}
    

    const btn = document.getElementById("make-pvs-button");


  

    const intersect = function(l1, l2) {
		const [v1, v2] = l1;
		const [v3, v4] = l2;
		const [x1,y1] = v1;
		const [x2,y2] = v2;
		const [x3,y3] = v3;
		const [x4,y4] = v4;

		// Check if none of the lines are of length 0
		if ((x1 === x2 && y1 === y2) || (x3 === x4 && y3 === y4)) {
				return false
		}

		const denominator = ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1))

		// Lines are parallel
		if (denominator === 0) {
				return false
		}

		const ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denominator
		const ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator

		// is the intersection along the segments
		if (ua < 0 || ua > 1 || ub < 0 || ub > 1) {
				return false
		}

		// Return a object with the x and y coordinates of the intersection
		const x = x1 + ua * (x2 - x1);
		const y = y1 + ua * (y2 - y1);

		return [x, y]
    }

    const make_infinite_line = function(start, angleRads) {
		const x = 100000*Math.cos(angleRads);
		const y = -100000*Math.sin(angleRads);
		let endx = start[0]+x;
		let endy = start[1]+y;
		if(Math.abs(Math.round(endx) - endx) < .001) {
			endx = Math.round(endx);
		}

		if(Math.abs(Math.round(endy) - endy) < .001) {
			endy = Math.round(endy);
		}
		const end = [endx, endy];

		
		const adj_start = [start[0]+0.01, start[1]+0.01];
		

		const line = [adj_start, end];
		
		//return line;
		return clip_line_against_bounds(line);

	}

	const clip_line_against_bounds = function (line) {
		const [start, end] = line;

		const [[minX, minY], [maxX, maxY]] = get_level_bounds(vertexes);

		const tl = [minX, minY];
		const tr = [maxX, minY];
		const bl = [minX, maxY];
		const br = [maxX, maxY];

		const intersection = (intersect(line, [tl, tr]) || // left side
					intersect(line, [tr, br]) || // right side
					intersect(line, [bl, br]) || // bottom side
					intersect(line, [tl, bl]));  // left side

		return [start, intersection || end]; 
		
		
    }
    
    
    


    const generate_penumbra_between_portals = function(src_portal, dst_portal) {
		const [sv1, sv2, src_tgt] = src_portal;
		const [dv1, dv2, dst_tgt] = dst_portal;

		const line1_start = sv1;
		const line1_end = dv2;

		const line2_start = sv2;
		const line2_end = dv1;
		

		const anti_penumbra_line_1 = [sv1, dv2];
		const anti_penumbra_line_2 = [sv2, dv1];
		const intersection_point = intersect(anti_penumbra_line_1, anti_penumbra_line_2);
		if(!intersection_point) {
			console.log("No intersection point found between lines " + anti_penumbra_line_1 + " and " + anti_penumbra_line_2);
			return undefined;
		}
		const line1_dy = -(line1_end[1] - line1_start[1]);
		const line1_dx = line1_end[0] - line1_start[0];
		const line1_angle = Math.atan2(line1_dy, line1_dx);

		const [inf_line_1_start,inf_line_1_end] = make_infinite_line(sv1, line1_angle);

		
		const line2_dy = -(line2_end[1] - line2_start[1]);
		const line2_dx = line2_end[0] - line2_start[0];
		const line2_angle = Math.atan2(line2_dy, line2_dx);

		const [inf_line_2_start,inf_line_2_end] = make_infinite_line(sv2, line2_angle);

		//const anti_penumbra_line_1 = [sv1,dv2];
		//const anti_penumbra_line_2 = [sv2,dv1];
		//const intersection_point = intersect(anti_penumbra_line_1, anti_penumbra_line_2);
		//if(!intersection_point) {
		//	console.log("No intersection point found between lines " + anti_penumbra_line_1 + " and " + anti_penumbra_line_2);
		//	return undefined;
		//}
		return {
			'src_portal': src_tgt, 
			'dst_portal': dst_tgt, 
			'intersection_point': intersection_point,
			'frustum_line_1': [intersection_point, inf_line_1_end],
			'frustum_line_2': [intersection_point, inf_line_2_end],
			'anti_penumbra_line_1': [sv1, dv2],
			'anti_penumbra_line_2': [sv2, dv1]
		};

    }
	/*
    const generate_frustum_from_penumbra = function(penumbra) {
		const {src_portal, dst_portal, line_1, line_2} = penumbra;
		const intersection_point = intersect(line_1, line_2);
		
		const [, line_1_2] = line_1;
		const [, line_2_2] = line_2;

		const line1_dy = line_1_2[1] - intersection_point[1];
		const line1_dx = line_1_2[0] - intersection_point[0];
		const line1_angle = Math.atan2(line1_dy, line1_dx);

		const [, inf_line_1_end] = make_infinite_line(intersection_point, line1_angle);

		
		const line2_dy = line_2_2[1] - intersection_point[1];
		const line2_dx = line_2_2[0] - intersection_point[0];
		const line2_angle = Math.atan2(line2_dy, line2_dx);

		const [, inf_line_2_end] = make_infinite_line(intersection_point, line2_angle);


		if(!intersection_point) {
			console.log("No intersection point found between lines " + line_1 + " and " + line_2);
			return undefined; 
			//throw new Error("No intersection point found between lines " + line1 + " and " + line2);
		}
		//const [line_1_1, line_1_2] = line_1;
		//const [line_2_1, line_2_2] = line_2;

		return {
				'src_portal': src_portal, 'dst_portal': dst_portal, 
				'line_1': [intersection_point, inf_line_1_end], 
				'line_2': [intersection_point, inf_line_2_end]
		}; 
	}
	*/
    

    const point_in_polygon = function(point, polygon) {
		let intersections = 0;
		const inf_line = make_infinite_line(point, 0);
		//const clipped_inf_line = clip_line_against_bounds(inf_line);
		//console.log("using test line: ", clipped_inf_line);
		polygon.forEach(function(line) {
			//const clipped_old_line = clip_line_against_bounds(line);
			//console.log("testing against unclipped line: ", line);
			//console.log("testing against line: ", clipped_old_line);
			if(//intersect(clipped_inf_line, clipped_old_line)) { 
			   intersect(inf_line, line)) {
				//console.log("intersects");
				intersections += 1;
			}
		});
		//console.log("got " + intersections + " intersections");

		return ((intersections & 0b1) == 1);
    }


    const get_line_slope = function([[v1x,v1y],[v2x,v2y]]) {
		return (v2y - v1y)/(v2x - v1x);
    }

    const get_line_y_intercept = function([[v1x,v1y],[v2x,v2y]], slope) {
		const dx_to_0 = -v1x;
		const dy_to_0 = dx_to_0 * slope;
		return v1y += dy_to_0;
    }

	const point_on_line  = function(point, line) {
		const point_x = point[0];
		const point_y = point[1];
		const slope = get_line_slope(line);
		const y_int = get_line_y_intercept(line, slope);
		let line_y_at_point_x;
		if(Number.isNaN(slope)) {
			line_y_at_point_x = y_int;
		} else {
			line_y_at_point_x = slope*point_x + y_int;
		}
		return (Math.abs(line_y_at_point_x - point_y) < 0.01);

	}

	const point_on_polygon_line = function(point, polygon) {
		return polygon.some(function(line) { 
			return point_on_line(point, line);	
		});
	}

	const line_on_line = function(line1, line2) {
		const [point1,point2] = line1;
		return point_on_line(point1, line2) && point_on_line(point2, line2);
	}
    
    
    
    const are_lines_coplanar = function(line1, line2) {
		// check if all four lines lie on the same plane
		const slope1 = get_line_slope(line1);
		const slope2 = get_line_slope(line2);
		if(Math.abs(slope1) !== Math.abs(slope2)) {
			return false;
		}

		const dx = line1[1][0] - line1[0][0];
		if(dx === 0) {
			// no y intercept, check x values
			return line1[1][0] === line2[0][0];
		}
		
		const y_int1 = get_line_y_intercept(line1, slope1);
		const y_int2 = get_line_y_intercept(line2, slope2);

		return (y_int1 === y_int2);
    }
	
	
    const frustum_within_frustum = function(new_frustum, old_frustum) {

		const new_l1 = new_frustum.frustum_line_1; 
		const new_l2 = new_frustum.frustum_line_2;
		
		const test_l1 = old_frustum.frustum_line_1;
		const test_l2 = old_frustum.frustum_line_2;
		
		console.log("comparing frustum: ", new_frustum);
		console.log("to: ", old_frustum);

		//if ((intersect(new_l1, test_l1) && !line_on_line(new_l1, test_l1)) ||
		//	(intersect(new_l1, test_l2) && !line_on_line(new_l1, test_l2)) ||
		//	(intersect(new_l2, test_l1) && !line_on_line(new_l2, test_l1)) ||
		//	(intersect(new_l2, test_l2) && !line_on_line(new_l2, test_l2))) {
		//	return true;
		//}

	
		//const { line_1, line_2 } = old_frustum;
		const [ , test_l1_v2] = test_l1;
		const [ , test_l2_v2] = test_l2;
		const end_cap_line = [test_l1_v2, test_l2_v2];
		const poly = [test_l1, test_l2, end_cap_line]; //line_1, line_2];

		
		console.log("testing point 1 " + new_l1[0]);
		const point_1_inside = (point_in_polygon(new_l1[0], poly) && !point_on_polygon_line(new_l1[0], poly));
		console.log("testing point 2 " + new_l1[1]);
		const point_2_inside = (point_in_polygon(new_l1[1], poly) && !point_on_polygon_line(new_l1[1], poly));
		console.log("testing point 3 " + new_l2[1]);
		const point_3_inside = (point_in_polygon(new_l2[1], poly) && !point_on_polygon_line(new_l2[1], poly));

		if(point_1_inside) {
			console.log("point l1[0] " + new_l1[0] + " inside");
		} else if (point_2_inside) {
			console.log("point l1[1] " + new_l1[1] + " inside");
		} else if (point_3_inside) {
			console.log("point l2[1] " + new_l2[1] + " inside");
		}

		const inside =  (point_1_inside || point_2_inside || point_3_inside);

		console.log("is inside: ", inside);
		return inside;
		/*
		if(intersections === 0 || intersections === 2) {
			return false;
		}
		
		return true;
		*/

    }


    

    const recursive_pvs = function(start_sect_num, cur_sect_num, portal_trail, cur_frustums, cur_pvs, draw) {
		// get portals in this sector
			
		const is_start_sector = cur_sect_num === start_sect_num;

		const last_portal = (portal_trail.length > 0 ? portal_trail[portal_trail.length-1] : undefined);

		//let cur_pvs = new Set();
		const cur_sector = sectors[cur_sect_num];
		
		const walls = get_walls_for_sector(cur_sector);

		walls.forEach(function(wall) {
			const [v1,v2,portal_tgt] = wall;
			if(portal_tgt === -1 || cur_pvs.has(portal_tgt)) {
				return;
			}

			//if(portal_tgt === 5) { return; }
			if(is_start_sector) {
				cur_pvs.add(portal_tgt);
				recursive_pvs(start_sect_num, portal_tgt, portal_trail.concat([wall]), cur_frustums, cur_pvs, draw);
				return;
			}
			

			//const any_backfacing_portals = portal_trail.some(function(test_portal) {
			//	return is_wall_backfacing(test_portal, wall);
			//});
	
				
			//if(any_backfacing_portals) {
				//return;
			//}
			
			console.log("generating path from " + last_portal + " to " + wall);
			const new_frustum = generate_penumbra_between_portals(last_portal, wall);
			if(new_frustum === undefined) {
				console.log("cannot draw anti-penumbra with intersection?");
				return;
			}
			const {anti_penumbra_line_1, anti_penumbra_line_2, frustum_line_1, frustum_line_2, intersection_point} = new_frustum;


			const [pl1_v1, ] = anti_penumbra_line_1;
			const [pl2_v1, ] = anti_penumbra_line_2;


			if(draw) {
				// draw first part of penumbra 
				draw_wall([pl1_v1, intersection_point, -1]); //l1v2, -1]);
				draw_wall([pl2_v1, intersection_point, -1]); //l2v2, -1]);
			}
			

			const cur_portal_line = [v1,v2];

			//const [last_v1,last_v2, last_portal_tgt] = last_portal;

			const any_coplanar_portals = portal_trail.some(function(test_portal) {
				const [tv1,tv2,tptl_tgt] = test_portal;
				const test_portal_line = [tv1,tv2];
				return are_lines_coplanar(cur_portal_line, test_portal_line, draw);
			});

			if(any_coplanar_portals) {
				console.log("got coplanar portals");
				return;
			}

			
			
			const within_frustums = cur_frustums.every(function(test_frustum) {
				return frustum_within_frustum(new_frustum, test_frustum);
			});
			

			const [fl1v1, fl1v2] = frustum_line_1;
			const [fl2v1, fl2v2] = frustum_line_2;
			if(within_frustums) {
				console.log("path is within frustums");
				if(draw) {
					// draw frustum and polygon
					draw_wall([fl1v1, fl1v2, -2]);
					draw_wall([fl2v1, fl2v2, -2]);
					draw_polygon([intersection_point, fl1v2, fl2v2]); //, fl2v1);
				}
				
				// add to pvs
				// recurse through portal
				// add to frustum
				const new_frustums = cur_frustums.concat([new_frustum]);
				console.log("now we have " + new_frustums.length + " frustums");
				cur_pvs.add(portal_tgt);
				recursive_pvs(start_sect_num, portal_tgt, portal_trail.concat([wall]), new_frustums, cur_pvs, draw);
			} else {
				if(draw) {
					draw_wall([fl1v1, fl1v2, 0]);
					draw_wall([fl2v1, fl2v2, 0]);
				}
			}
					
		});
    }


    
    btn.onclick = function() {
		const select = document.getElementById("selected-sector");
		const sector = parseInt(select.value);

		const pvs = new Set();
		pvs.add(sector);
		draw_map();
		recursive_pvs(sector, sector, [], [], pvs, true);
		display_pvs(pvs);
    }

    const find_sector_for_screen_position = function(v) {
		const world_v = untrans_coord(v);
		const [x, y] = world_v;
		const bounds = get_level_bounds(vertexes);
		const [[minX,minY], [maxX, maxY]] = bounds;
		console.log(bounds);
		if((x < minX) || (x > maxX) || (y < minY) || (y > maxY)) {
			return undefined;
		}


		for(var i = 0; i < sectors.length; i++) {
			const sect_num = sectors[i][2];
			const polygon = get_walls_for_sector(sectors[sect_num]);

			if(point_in_polygon(world_v, polygon)) {
			return sect_num;
			}
		}
		return undefined;	
    }
    
    canvas.onmousemove = function(evt) {
		draw_map();

		

		
		var rect = canvas.getBoundingClientRect();
			
		const x = evt.clientX - rect.left;
		const y = evt.clientY - rect.top;

		const sect_num = find_sector_for_screen_position([x,y]);
		console.log("sect num: ", sect_num);
		if(sect_num === undefined) {
			return;
		}

		const pvs = new Set();
		pvs.add(sect_num);
		recursive_pvs(sect_num, sect_num, [], [], pvs, false);

		pvs.forEach(function (sect_num) {
			const sector_verts = get_vertexes_for_sector(sectors[sect_num]);
			draw_polygon(sector_verts);
		});
	}
		
	draw_map();
		
    let angle = 0;
    const draw_new_inf_line = function() {
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	const infinite_line = make_infinite_line([3, 4], angle);

	
	draw_sectors();
	
	angle = angle+.01;
	
	if(infinite_line) {
	    const [v1, v2] = infinite_line;
	    draw_wall([v1, v2, -1]);
	}
	requestAnimationFrame(draw_new_inf_line);
	
    }

   // requestAnimationFrame(draw_new_inf_line);
    
    
    
})();
