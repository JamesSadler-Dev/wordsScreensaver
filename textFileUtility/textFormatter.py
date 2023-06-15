import sys
import re

def main():
    command(sys.argv[1],sys.argv[2])

def command(filename,outputname):
    words= []
    filename= str(filename)
    outputname = str(outputname)
    wordsSet= set()
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


    with open(outputname,"w") as outputFile: 
        for word in words:
            if len(word) > 1:
                outputFile.write((str(word).strip()) + "\n")

    with open("duplicates.txt","w") as outputFile: 
        for word in duplicates:
            if len(word) > 1:
                outputFile.write((str(word).strip()) + "\n")   

    with open("masterListBackup.txt","w") as outputFile: 
        counter=1

        for word in words:
            if len(str(word).strip()) > 1:
                outputFile.write("{:45s}".format((str(word).strip())) + "    \t    ") 
                outputFile.write("\n")
                counter+=1


    with open("input.txt","w") as outputFile: 

        for word in words:
            if len(str(word).strip()) > 1:
                outputFile.write("{:45s}".format((str(word).strip())) + "    \t    ") 
                outputFile.write("\n")

    sys.exit(0)    

if __name__ == "__main__":
    main()