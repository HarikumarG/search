#include <jni.h>
#include "FolderSearch.h" 
#include <iostream>
#include <dirent.h>
#include <ctime>
#include <sys/stat.h>
#include <vector>
#include <fstream>
#include <time.h>
using namespace std;
unsigned char isFile = 0x8;
unsigned char isFolder = 0x4;

struct fileinfo {
   string fname;
   string path;
   long long fsize;
   string ctime;
   string mtime;
};

vector<fileinfo> files;
vector<fileinfo>::iterator it;
vector<string> filenames;

void checkdir(string path,string pat) 
{
   DIR *dr;
   struct dirent *en;
   dr = opendir(path.c_str());
   if (dr) 
   {
      while ((en = readdir(dr)) != NULL) 
      {
         if(en->d_type == isFile) 
         {
            string res = en->d_name;
            if(res.find(pat) != -1) {
               string file = path+"/"+res;
               struct stat fileInfo;
               if(stat(file.c_str(),&fileInfo) != 0) {
                  cout<<"Error in getting info: "+file<<endl;
               } else {
                  struct fileinfo f;
                  f.fname = res;
                  f.path = path;
                  f.fsize = fileInfo.st_size;
                  f.ctime = ctime(&fileInfo.st_ctime);
                  f.mtime = ctime(&fileInfo.st_mtime);
                  files.push_back(f);
               }

            }
         } 
         else if(en->d_type == isFolder) 
         {
            string res = en->d_name;
            if(res != "." && res != "..") 
            {
               string fol = path+"/"+res;
               checkdir(fol,pat);
            }
         }
      }
      closedir(dr);
   }
}

void createMetaFile()
{
   time_t  my_time = time(NULL);
   string foldername = ctime(&my_time);
   string fol = "../"+foldername;
   if(files.size() > 0) {
      int check = mkdir(fol.c_str(),0777);
      if(!check) {
         for(it = files.begin(); it != files.end(); it++) 
         {
            string f;
            int pos = it->fname.find(".");
            if(pos != -1) 
            {
               string fi = it->fname.substr(0,pos);
               string ext = it->fname.substr(pos+1);
               f = fi+"_"+ext+"_meta.txt";
            }
            else 
               f = it->fname+"_meta.txt";

            string filepath = "../"+foldername+"/"+f;
            ofstream file(filepath.c_str());
            if(!file) 
            {  
               cout<<"Error in creating file: "+filepath<<endl;
            } 
            else 
            {
               file << "FileName:   "+it->fname << endl;
               file << "FilePath:   "+it->path << endl;
               file << "Size in Bytes:   "+to_string(it->fsize)<< endl;
               file << "Created time:   "+it->ctime;
               file << "Modified time:   "+it->mtime << endl;
               file.close();
               filenames.push_back(f);
            }

         }
      } 
      else {
         cout<<"Error in creating directory"<<endl;
      }
   }
   else {
      cout<<"Invalid path or No such files exist"<<endl;
   }
}

JNIEXPORT jobjectArray JNICALL Java_FolderSearch_startSearch(JNIEnv *env, jobject thisObj,jstring path,jstring pat)
{
   const char *p = env->GetStringUTFChars(path,NULL);
   const char *pt = env->GetStringUTFChars(pat,NULL);
   checkdir(p,pt);
   createMetaFile();

   jobjectArray res = (jobjectArray) env->NewObjectArray(filenames.size(), env->FindClass("java/lang/String"), NULL);

   for(int i = 0; i < filenames.size(); i++) {
      string temp = filenames[i];
      env->SetObjectArrayElement(res,i,env->NewStringUTF(temp.c_str()));
   }
   return res;
}