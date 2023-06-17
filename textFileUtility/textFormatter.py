import sys
import re


def main() -> None:
    command(sys.argv[1],sys.argv[2])


def command(filename:str,outputname:str) -> None:
    words= []
    filename= str(filename)
    outputname = str(outputname)
    duplicates= set()
    seen = set()

    ##some argument parsing
    if not sys.argv[1]:
        raise ValueError("Expected 2 Args got 0, Usage: inputfile.txt outputfile.txt")
    elif not sys.argv[2]:
        raise ValueError("Expected 2 Args got 1, Usage: inputfile.txt outputfile.txt")
    elif not filename.endswith(".txt") or not outputname.endswith(".txt"):
        raise ValueError("Usage error: Must be text files")
    
    ## Process the text file
    with open(f"{filename}","r") as myFile:
        lines = [re.split("\t",str(line).strip()) for line in myFile]
        
    i=0
    while i < len(lines):
        currentLine= lines[i]
        if len(currentLine) > 1:
            j=0
            while j < len(lines[i]):
                word= lines[i][j]
                wordUpper= word.upper().strip()

                if (wordUpper not in seen):
                    words.append(word)
                    seen.add(wordUpper)
                    j+=1
                else:
                    duplicates.add(word)  
                    j+=1  
        else:
            word= lines[i][0]
            wordUpper= word.upper().strip()

            if (wordUpper not in seen):
                words.append(word)
                seen.add(wordUpper)
            else:
                duplicates.add(word)  
            i+=1


    writeFile(f"{outputname}",words)
    writeFile("duplicates.txt",duplicates)
    writeFile("backup.txt",words)
    writeFile("input.txt",words)

    sys.exit(0)    


def writeFile(fileName:str,words:list) -> None: 
    with open(fileName,"w") as outputFile: 
        for word in words:
            word = str(word).strip()
            if len(word) > 1:
                outputFile.write("{:s}{:s}".format(word,"\t\n"))

if __name__ == "__main__":
    main()
