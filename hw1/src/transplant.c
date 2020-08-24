#include "const.h"
#include "transplant.h"
#include "debug.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif

/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the pathname of the current file or directory
 * as well as other data have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/*
 * A function that returns printable names for the record types, for use in
 * generating debugging printout.
 */
static char *record_type_name(int i) {
    switch(i) {
    case START_OF_TRANSMISSION:
	return "START_OF_TRANSMISSION";
    case END_OF_TRANSMISSION:
	return "END_OF_TRANSMISSION";
    case START_OF_DIRECTORY:
	return "START_OF_DIRECTORY";
    case END_OF_DIRECTORY:
	return "END_OF_DIRECTORY";
    case DIRECTORY_ENTRY:
	return "DIRECTORY_ENTRY";
    case FILE_DATA:
	return "FILE_DATA";
    default:
	return "UNKNOWN";
    }
}

/*
 * @brief  Initialize path_buf to a specified base path.
 * @details  This function copies its null-terminated argument string into
 * path_buf, including its terminating null byte.
 * The function fails if the argument string, including the terminating
 * null byte, is longer than the size of path_buf.  The path_length variable 
 * is set to the length of the string in path_buf, not including the terminating
 * null byte.
 *
 * @param  Pathname to be copied into path_buf.
 * @return 0 on success, -1 in case of error
 */
int get_path_buf_size(){
    char * it = path_buf;
    int ans=0;
    while(*it){
        ans++;
        if(ans>=PATH_MAX){
            return -1;
        }
        it++;
    }

    return ans;
}

int get_string_len(char * p){
    int ans=0;
    while(*p){
        ans++;
        p++;
    }
    return ans;
}

int path_init(char *name) {
    debug("Path before %s  paMAX LENGTH %d  namemax %d",path_buf, PATH_MAX, NAME_MAX);
    char * it = path_buf;
    int i=0;
    while(*name){
        *it=*name;
        name++;
        it++;
        i++;
        if(i>=PATH_MAX-1){
            debug("ABORT");
            return -1;
        }
    }
    
    (*it) = '\0';
    debug("Path After %s",path_buf);
    path_length  = get_path_buf_size();
    if(path_length==-1){
        debug("ABORT");
        return -1; 
    }
    return 0;
}

/*
 * @brief  Append an additional component to the end of the pathname in path_buf.
 * @details  This function assumes that path_buf has been initialized to a valid
 * string.  It appends to the existing string the path separator character '/',
 * followed by the string given as argument, including its terminating null byte.
 * The length of the new string, including the terminating null byte, must be
 * no more than the size of path_buf.  The variable path_length is updated to
 * remain consistent with the length of the string in path_buf.
 * 
 * @param  The string to be appended to the path in path_buf.  The string must
 * not contain any occurrences of the path separator character '/'.
 * @return 0 in case of success, -1 otherwise.
 */
int path_push(char *name) {
    debug("Path Buf before %s",path_buf);
    char * it = path_buf;
    int l  = get_string_len(name);

    if((path_length + 1 + l)>=PATH_MAX-1){
        debug("ABORT");
        return -1;
    }

    while(*it)it++;
    *it= '/';
    it++;
    while(*name){
        *it=*name;
        it++;
        name++;
    }
    *it = '\0';

    debug("Path Buf  after %s",path_buf);
    path_length  = get_path_buf_size();
    if(path_length==-1){
        debug("ABORT");
        return -1; 
    }
    return 0;
}

/*
 * @brief  Remove the last component from the end of the pathname.
 * @details  This function assumes that path_buf contains a non-empty string.
 * It removes the suffix of this string that starts at the last occurrence
 * of the path separator character '/'.  If there is no such occurrence,
 * then the entire string is removed, leaving an empty string in path_buf.
 * The variable path_length is updated to remain consistent with the length
 * of the string in path_buf.  The function fails if path_buf is originally
 * empty, so that there is no path component to be removed.
 *
 * @return 0 in case of success, -1 otherwise.
 */
int path_pop() {
    // To be implemented.
     debug("Inside Path POp");
     debug("Before %s \n",path_buf);
     if(path_length <= 0){
        debug("ABORT");
        return -1;
     }

     char * temp = path_buf + path_length -1;
     while((*temp)!='/' && temp!=path_buf){
        temp--;
        path_length--;
     }
     *temp='\0';
     path_length--;
     debug("After %s \n",path_buf);
    return 0;
}

/**
 * @brief Serializes a tree of files and directories, writes
 * serialized data to standard output.
 * @details This function assumes path_buf has been initialized with the pathname
 * of a directory whose contents are to be serialized.  It traverses the tree of
 * files and directories contained in this directory (not including the directory
 * itself) and it emits on the standard output a sequence of bytes from which the
 * tree can be reconstructed.  Options that modify the behavior are obtained from
 * the global_options variable.
 *
 * @return 0 if serialization completes without error, -1 if an error occurs.
 */
int putmagic(){
    int f1 = putchar(MAGIC0);
    int f2 = putchar(MAGIC1);
    int f3 = putchar(MAGIC2);
    if(f1==EOF || f2==EOF || f3==EOF){
        debug("ABORT");
        return -1;
    }
    return 0;
}
int put4byte(unsigned long long depth){
     int f1 = putchar(depth>>24 & 0xff);
     int f2 = putchar(depth>>16 & 0xff);
     int f3 = putchar(depth>>8 & 0xff);
     int f4 = putchar(depth & 0xff);
     if(f1==EOF || f2==EOF || f3==EOF || f4==EOF ){
        debug("ABORT");
        return -1;
    }
     return 0;
}
int put8byte(unsigned long long size){
     int f1 = putchar(size>>56 & 0xff);
     int f2 = putchar(size>>48 & 0xff);
     int f3 = putchar(size>>40 & 0xff);
     int f4 = putchar(size>>32 & 0xff);
     int f5 = putchar(size>>24 & 0xff);
     int f6 = putchar(size>>16 & 0xff);
     int f7 = putchar(size>>8 & 0xff);
     int f8 = putchar(size & 0xff);
     if(f1==EOF || f2==EOF || f3==EOF || f4==EOF || f5==EOF || f6==EOF || f7==EOF || f8==EOF){
        debug("ABORT");
        return -1;
    }
     return 0;
}
int  putstring(char * name){
    while(*name){
        int f = putchar(*name);
        if(f==EOF){
            debug("ABORT");
            return -1;
        }
        name++;
    }
    return 0;
}
int data_entry(unsigned long long depth, unsigned long long size, unsigned long long mode, unsigned long long sze, char * name ){
    int flag = putmagic();
    if(flag==-1)return -1;
    flag = putchar(DIRECTORY_ENTRY);
    if(flag==-1)return -1;
    flag = put4byte(depth);
    if(flag==-1)return -1;
    flag = put8byte(size + 12 + get_string_len(name));
    if(flag==-1)return -1;
    debug("Sizeof name %d %s",get_string_len(name),name);

    flag = put4byte(mode);
    if(flag==-1)return -1;
    flag = put8byte(sze);
    if(flag==-1)return -1;
    flag = putstring(name);
    if(flag==-1)return -1;
    return 0;
}
/*
 * @brief  Serialize the contents of a file as a single record written to the
 * standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * file to be serialized.  It serializes the contents of that file as a single
 * FILE_DATA record emitted to the standard output.
 *
 * @param depth  The value to be used in the depth field of the FILE_DATA record.
 * @param size  The number of bytes of data in the file to be serialized.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open the file, too many or not enough data bytes read
 * from the file, and I/O errors reading the file data or writing to standard output.
 */
int serialize_file(int depth, off_t size) {
    int f1  = putmagic();
    int f2 = putchar(FILE_DATA);
    int f3 = put4byte(depth);
    int f4 = put8byte(size + HEADER_SIZE);
    if(f1==-1 || f2==-1 || f3==-1 || f4==-1 ){
        debug("ABORT");
        return -1;
    }

    FILE *f = fopen(path_buf,"r");
    if(f==NULL){
        debug("ABORT");
        return -1;
    }
    char c;
    while((c = fgetc(f))!=EOF){
        f1 = putchar(c);
        if(f1==-1){
            debug("ABORT");
            return -1;
        }
    }
    fclose(f);
    return 0;
}
/*
 * @brief  Serialize the contents of a directory as a sequence of records written
 * to the standard output.
 * @details  This function assumes that path_buf contains the name of an existing
 * directory to be serialized.  It serializes the contents of that directory as a
 * sequence of records that begins with a START_OF_DIRECTORY record, ends with an
 * END_OF_DIRECTORY record, and with the intervening records all of type DIRECTORY_ENTRY.
 *
 * @param depth  The value of the depth field that is expected to occur in the
 * START_OF_DIRECTORY, DIRECTORY_ENTRY, and END_OF_DIRECTORY records processed.
 * Note that this depth pertains only to the "top-level" records in the sequence:
 * DIRECTORY_ENTRY records may be recursively followed by similar sequence of
 * records describing sub-directories at a greater depth.
 * @return 0 in case of success, -1 otherwise.  A variety of errors can occur,
 * including failure to open files, failure to traverse directories, and I/O errors
 * that occur while reading file content and writing to standard output.
 */
int serialize_directory(int depth) {
    
    DIR *dir = opendir(path_buf);
    if(dir==NULL){
        debug("ABORT");
        return -1;
    }
    depth++;
    unsigned long long size = HEADER_SIZE;
    int f1  = putmagic();
    int f2 = putchar(START_OF_DIRECTORY);
    int f3 = put4byte(depth);
    int f4 = put8byte(size);
    if(f1==-1 || f2==-1 || f3==-1 || f4==-1 ){
        debug("ABORT");
        return -1;
    }

    struct dirent * de;
    while((de = readdir(dir))!=NULL){
        if(*(de->d_name)=='.' && (*((de->d_name)+1)=='\0' || *((de->d_name)+1)=='.'))continue;
        if(path_push(de->d_name)!=-1){
            struct stat st;
            int flag = stat(path_buf, &st);
            if(flag==-1){
                    debug("ABORT");
                    return -1;
            }
            unsigned long long mode = st.st_mode;
            unsigned long long sze = st.st_size;
            int is_directory = 0;
            if(st.st_mode & S_IFDIR)
                is_directory = 1;
            debug("Path %s and is directory %d %ld \n",de->d_name,st.st_mode, st.st_size);
            flag = data_entry(depth,size,mode,sze,de->d_name);
            if(flag==-1){
                    debug("ABORT");
                    return -1;
            }
            if(is_directory){
                flag = serialize_directory(depth);
                if(flag==-1){
                    debug("ABORT");
                    return -1;
                }
            }else{ 
                flag = serialize_file(depth,sze);
                if(flag==-1){
                    debug("ABORT");
                    return -1;
                }
            }
            path_pop();
        }else{
            debug("ABORT");
            return -1;
        }
    }
    f1 = putmagic();
    f2 = putchar(END_OF_DIRECTORY);
    f3 = put4byte(depth);
    f4 = put8byte(size);
    if(f1==-1 || f2==-1 || f3==-1 || f4==-1 ){
        debug("ABORT");
        return -1;
    }
    depth--;
    
    closedir(dir);
    return 0;
}
int serialize() {
    // To be implemented.
    unsigned long long depth = 0;
    unsigned long long size = HEADER_SIZE;
    debug("Inside Serialize");

    int f1 = putmagic();
    int f2 = putchar(START_OF_TRANSMISSION);
    int f3 = put4byte(depth);
    int f4 = put8byte(size);
    if(f1==-1 || f2==-1 || f3==-1 || f4==-1 ){
        debug("ABORT");
        return -1;
    }

    f1 = serialize_directory(depth);
    if(f1==-1){
        debug("ABORT");
        return -1;
    }

    f1  = putmagic();
    f2 = putchar(END_OF_TRANSMISSION);
    f3 = put4byte(depth);
    f4 = put8byte(size);
    if(f1==-1 || f2==-1 || f3==-1 || f4==-1 ){
        debug("ABORT");
        return -1;
    }

    return 0;
}

int getmagic(){
    debug("Magic");
    int m0 = getchar();
    int m1 = getchar();
    int m2 = getchar();
    if(m0==EOF || m1==EOF || m2==EOF){
        debug("ABORT EOF");
        return -1;
    }
    if((m0 & MAGIC0) && (m1 & MAGIC1) && (m2 & MAGIC2))return 0;
    debug("ABORT");
    return -1;
}
unsigned long get4byte(){
     unsigned long i=0;
     int counter = 4;
     while(counter--){
        int c = getchar();
        if(c==EOF){
            debug("ABORT");
            return -1;
        }
        i = i | (unsigned long)c<<(counter*8);
     }
     debug("%lu",i);
     return i;
}
unsigned long get8byte(){
     unsigned long i=0;
     int counter = 8;
     while(counter--){
        int c = getchar();
        if(c==EOF){
            debug("ABORT");
            return -1;
        }
        i = i | (unsigned long)c<<(counter*8);
     }
     // i = i  |  (unsigned long)getchar()<<56;
     // i = i  |  (unsigned long)getchar()<<48;
     // i = i  |  (unsigned long)getchar()<<40;
     // i = i  |  (unsigned long)getchar()<<32;
     // i = i  |  (unsigned long)getchar()<<24;
     // i = i  |  (unsigned long)getchar()<<16;
     // i = i  |  (unsigned long)getchar()<<8;
     // i = i  |  getchar();
     debug("%lu",i);
     return i;
}
int check_header(int req_record_type, int req_depth){
    int i = getmagic();
    int record_type = getchar();
    unsigned long depth = get4byte();
    unsigned long record_size = get8byte();
    if(i==0 && record_type==req_record_type && depth==req_depth && record_size==HEADER_SIZE)return 0;
    debug("ABORT");
    return -1;
}
int remove_header(int req_record_type, int req_depth){
    int i = getmagic();
    int record_type = getchar();
    unsigned long depth = get4byte();
    unsigned long record_size = get8byte();
    record_size -= HEADER_SIZE;
    if(i==0 && record_type==req_record_type && depth==req_depth && record_size>=0)return record_size;
    debug("ABORT");
    return -1;
}

int deserialize_file(int depth){
    debug("Inside def");
    int data_size = remove_header(FILE_DATA,depth);
    if(data_size==-1){
        debug("ABORT");
        return -1;
    }
    debug("Data Size %d",data_size);
    FILE *f = fopen(path_buf,"w");
    if(f==NULL){
        debug("FILE NOT OPENING %s",path_buf);
        debug("ABORT");
        return -1;
    }
    while(data_size){
        int i = getchar();
        if(i==EOF){
            debug("ABORT EOF");
            return -1;
        }
        fputc(i,f);
        data_size-=1;
    }
    fclose(f);
    return 0;
}

int deserialize_directory(int depth) {
    int flag = check_header(START_OF_DIRECTORY,depth);
    if(flag==-1){
        debug("ABORT");
        return -1;
    }
    while(1){
        int i = getmagic();
        int record_type = getchar();
        unsigned long depth = get4byte();
        unsigned long record_size = get8byte();
        if(i==-1 || record_type==-1 || depth == -1 || record_size==-1){
            debug("ABORT");
            return -1;
        }
        if(record_type==END_OF_DIRECTORY)
            break;
        unsigned long mode = get4byte();
        unsigned long file_or_directory_size = get8byte();
        if(mode ==-1 || file_or_directory_size ==-1){
            debug("ABORT");
            return -1;
        }
        record_size-=(HEADER_SIZE+12);
        char * c = name_buf;
        if(record_size > NAME_MAX){
            debug("ABORT RECORD size %lu",record_size);
            return -1;
        }
        while(record_size--){
            int a = getchar();
            *c = a;
            c++;
        }
        *c = '\0';
        int p = path_push(name_buf);
        if(p==-1){
            debug("ABORT");
            return -1;
        }
        if(global_options == 4){
            debug("CHECK FOR DUPLICATES");
            struct stat st;
            if(!stat(path_buf, &st) ){
                if((mode & S_IFDIR) && (st.st_mode & S_IFDIR)){
                    debug("DUPLICTAE DIRECTORY");
                }else if((mode & S_IFREG) && (st.st_mode & S_IFREG)){
                    debug("DUPLICTAE FILE");
                }
                debug("ABORT");
                return -1;
            }
        }
        if(mode & S_IFDIR){
            int flag = mkdir(path_buf,0700);
            if(flag==-1 && global_options!=12){
                debug("ABORT");
                return -1;
            }
            flag = chmod(path_buf, mode & 0777);
            if(flag==-1){
                debug("ABORT");
                return -1;
            }
            int i = deserialize_directory(depth+1);
            if(i==-1){
                debug("ABORT");
                return -1;
            }
        }else{
            
            int flag = deserialize_file(depth);
            if(flag==-1){
                debug("ABORT");
                return -1;
            }
            flag = chmod(path_buf,mode & 0777);
            if(flag==-1){
                debug("ABORT");
                return -1;
            }
            
        }
        path_pop();
    }
    return 0;
}
int deserialize() {
    mkdir(path_buf,0700); //Create Directory if it doesnt exist
    int depth=0;
    int flag  = check_header(START_OF_TRANSMISSION,depth);
    if(flag==-1){
        debug("ABORT");
        return -1;
    }
    debug("%d",flag);
    flag = deserialize_directory(depth+1);
    if(flag==-1){
        debug("ABORT");
        return -1;
    }
    flag = check_header(END_OF_TRANSMISSION,depth);
    if(flag==-1){
        debug("ABORT");
        return -1;
    }
    return 0;
}

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 0 if validation succeeds and -1 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variable "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 0 if validation succeeds and -1 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int compstr(char * a, char * b){
    while(*a && *b){
        if(*a!=*b) return 0;
        a++;
        b++;
    }
    if(*a=='\0' && *b=='\0')return 1;
    return 0;
}

int validargs(int argc, char **argv)
{   
    if(argc<=1){
        return -1;
    }
    //First argument is filename so we skip it
    ++argv;
    char *sflag = "-s";
    char *dflag = "-d";
    char *cflag = "-c";
    char *hflag = "-h";
    char *pflag = "-p";

    debug("param %s",*argv);

    if(compstr(*argv,hflag)){
        global_options=1;
        return 0;
    }
    else if(compstr(*argv,sflag)){
        debug("GOT S");
        global_options=2;
        if(argc==2){
             debug("serialize with path current dir");
             path_init(".");
             return 0;
        }
        else if(argc==4){
            ++argv;
            if(compstr(*argv,pflag)){
                debug("serialize with path %s",*(argv+1));
                path_init(*(argv+1));
                return 0;
            }
        }
        return -1;
    }
    else if(compstr(*argv,dflag)){
        debug("GOT d");
        ++argv;
        if(argc==2){
            global_options=4;
            debug("Deserialize with path NULL");
            return 0;
        }
        else if(argc==3){
            if(compstr(*argv,cflag)){
                global_options=12;
                debug("Deserialize with path NULL");
                path_init(".");
                return 0;
            }
        }
        else if(argc==4){
            if(compstr(*argv,pflag)){
                global_options=4;
                debug("Deserialize with path %s ",*(argv+1));
                int i = path_init(*(argv+1));
                if(i==-1){
                    debug("ABORT");
                    return -1;
                }
                return 0;
            }
        }
        else if(argc==5){
            if(compstr(*argv,cflag) && compstr(*(argv+1),pflag)){
                global_options=12;
                int i = path_init(*(argv+2));
                if(i==-1){
                    debug("ABORT");
                    return -1;
                }
                debug("Deserialize with path %s ",*(argv+2));
                return 0;
            }
            else if(compstr(*argv,pflag) && compstr(*(argv+2),cflag)){
                global_options= 12;
                int i = path_init(*(argv+1));
                if(i==-1){
                    debug("ABORT");
                    return -1;
                }
                debug("Deserialize with path %s ",*(argv+1));
                return 0;
            }
        }
        return -1;
    }

    return -1;
}