#include<bits/stdc++.h>
#include <jni.h>
#include "KeywordSearch.h"
using namespace std;

JNIEXPORT void JNICALL Java_KeywordSearch_startKeySearch (JNIEnv *env, jobject thisObj, jstring path, jstring filename, jstring keyword) {

	const char *p = env->GetStringUTFChars(path,NULL);
	const char *fn = env->GetStringUTFChars(filename,NULL);
	const char *key = env->GetStringUTFChars(keyword,NULL);
	
	jclass thisclass = env->GetObjectClass(thisObj);
	jmethodID callFileWrite = env->GetMethodID(thisclass,"filewrite","(Ljava/lang/String;)V");

	if(NULL == callFileWrite) {
		cout<<"Error in initializing method in java from cpp"<<endl;
	} 
	else 
	{
		string p1 = p;
		string fn1 = fn;
		string finalpath = p1+"/"+fn1;
		string k = key;
		fstream file;
		file.open(finalpath,ios::in);
		if(file.is_open()) {
			string tp;
			while(getline(file,tp)) {
				if(tp.find(k) != -1) {
					jstring text = env->NewStringUTF(tp.c_str());
					env->CallVoidMethod(thisObj,callFileWrite,text);
				}
			}
		} else {
			cout<<"Error in opening file: "+finalpath<<endl;
		}
	}
}