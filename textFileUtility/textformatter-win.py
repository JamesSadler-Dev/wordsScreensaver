import sys
import re
import time
import os

def main():
    command()

def command():
    words= []


    
    ## Process the input.txt file to system32 folder windows
    with open(f"./input.txt","r") as myFile:
        lines = [re.split(",",str(line).strip()) for line in myFile]
        
        i=0
        while i < len(lines):
            if len(lines[i]) > 1:
                j=0
                while j < len(lines[i]):
                    words.append(lines[i][j])
                    j+=1
            else:
                words.append(str(lines[i][0]))
            i+=1


    with open(os.path.join(os.environ['USERPROFILE'], "Documents"),"w") as outputFile: 
        for word in words:
            if len(word) > 1:
                outputFile.write((str(word).strip()) + "\n")
                print("success")
                time.sleep(1)

    
    sys.exit(0)

if __name__ == "__main__":
    main()