def gen_table():
    pass

if __name__ == '__main__':
    for z_12_4 in range(65536):
        if z_12_4 == 0:
            print(0, end=", ")
            continue

        val = int(1<<26)//z_12_4
        print(val, end=", ")


        if z_12_4 % 50 == 0:
            print("")

