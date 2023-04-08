    if (os.name == "posix"):  #If the OS used is Ubuntu 
                string +="\/main.cpp SingleCycle.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "\/./a.out"
                os.system(string2)
            elif (os.name == "nt"):   #If the OS used is Windows
                string +="\main.cpp SingleCycle.cpp -I ../include"
                string = "g++ " + string
                os.system(string)
                string2 += "./a.exe"
                os.system(string2)