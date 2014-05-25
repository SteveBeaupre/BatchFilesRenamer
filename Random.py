import random, os.path

ScriptName = "Random"
InitialDirectory = ""

Input  = ""
Output = ""

##-------------------------------------------------------------------------------------------------------------------
##- Generate a random character
##-------------------------------------------------------------------------------------------------------------------
def GenRandChar():
    r = random.randrange(0,1)
    
    s = ""
    if(r == 0):
        s = chr(random.randrange(65, 65 + 26))

    if(r == 1):
        s = chr(random.randrange(48, 48 + 10))

    return s

##-------------------------------------------------------------------------------------------------------------------
##- Convert the file name (Called from c++)
##-------------------------------------------------------------------------------------------------------------------
def Convert():
    global Input
    global Output
    
    FileName = ""

    for i in range(16):
        FileName = FileName + GenRandChar()

    FileExt = os.path.splitext(Input)[1]
    #print(FileExt)
    
    Output = FileName + FileExt

#Convert()
#print(Output)
