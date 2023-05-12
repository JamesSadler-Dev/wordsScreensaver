import sys
import re

def main():
    command(sys.argv[1],sys.argv[2])

def command(filename,outputname):
    words= []
    filename= str(filename)
    outputname = str(outputname)

    ##some argument parsing
    if not sys.argv[1]:
        raise ValueError("Expected 2 Args got 0, Usage: inputfile.txt outputfile.txt")
    elif not sys.argv[2]:
        raise ValueError("Expected 2 Args got 1, Usage: inputfile.txt outputfile.txt")
    elif not filename.endswith(".txt") or not outputname.endswith(".txt"):
        raise ValueError("Usage error: Must be text files")
    
    ## Process the text file
    with open(f"{filename}","r") as myFile:
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

    with open(outputname,"w") as outputFile: 
        for word in words:
            if len(word) > 1:
                outputFile.write((str(word).strip()) + "\n")
    sys.exit(0)

if __name__ == "__main__":
    main()