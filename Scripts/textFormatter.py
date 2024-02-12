import sys
import re
import os


def main() -> None:
    command(sys.argv[1],sys.argv[2])
    sys.exit(0) 

def command(filename:str,outputname:str) -> None:
    words= []
    filename= str(filename)
    outputname = str(outputname)
    reused= set()
    seen = set()

    ##some argument parsing
    if not sys.argv[1]:
        raise ValueError("Expected 2 Args got 0, Usage: inputfile.txt outputfile.txt")
    elif not sys.argv[2]:
        raise ValueError("Expected 2 Args got 1, Usage: inputfile.txt outputfile.txt")
    elif not filename.endswith(".txt") or not outputname.endswith(".txt"):
        raise ValueError("Usage error: Must be text files")
    
    
    words_path:str = "./outputs/all/words.txt"

    
    process_file(filename,seen,words,reused,f"./outputs/{outputname}","./outputs/duplicates.txt","current")
    ## Write over the original file to remove duplicates
    write_file(filename,words)
    ##  Process file of all for duplicates
    process_file(words_path,seen,words,reused,words_path,"./outputs/duplicatesAll.txt","all")

       

def process_file(filename,seen_tokens,words_to_write,
                    reused_tokens,output_path,
                    duplicates_file,folder):
        
        with open(f"{filename}","r") as myFile:
            lines = [re.split("\t",str(line).strip()) for line in myFile]
            
        i=0
        while i < len(lines):
            current_line= lines[i]
            if len(current_line) > 1:
                j=0
                while j < len(lines[i]):
                    word= lines[i][j]
                    word_upper= word.upper().strip()

                    if (word_upper not in seen_tokens):
                        words_to_write.append(word)
                        seen_tokens.add(word_upper)
                        j+=1
                    else:
                        reused_tokens.add(word)  
                        j+=1  
            else:
                word= lines[i][0]
                word_upper= word.upper().strip()

                if (word_upper not in seen_tokens):
                    words_to_write.append(word)
                    seen_tokens.add(word_upper)
                else:
                    reused_tokens.add(word)  
                i+=1

        ANSI_GREEN="\u001b[32m"
        ANSI_RESET="\u001b[0m"
        output_string= "{:s}\n{:30s} TO:\t {:s}\n".format(ANSI_GREEN,filename,output_path)     
        output_line2= "{:s}\b{:7s}, DUPLICATES ADDED\n".format(ANSI_RESET,"LINES")
        output_line3= "\b{:5d}  ,\t{:d}".format(len(words_to_write),len(reused_tokens))
        print(output_string,output_line2,output_line3)      
        
        if os.path.exists(os.path.join(f"./outputs/{folder}")):
            write_file(f"./outputs/{folder}/words.txt",words_to_write)
        else:
            os.mkdir(f"./outputs/{folder}")
            write_file(f"./outputs/{folder}/words.txt",words_to_write)
        
        write_file(duplicates_file,reused_tokens)



def write_file(file_name:str,words_to_write:list) -> None: 
    with open(f"{file_name}","w") as outputFile: 
        for word in words_to_write:
            word = str(word).strip()
            if len(word) > 1:
                outputFile.write("{:s}{:s}".format(word,"\t\n"))

if __name__ == "__main__":
    main()
