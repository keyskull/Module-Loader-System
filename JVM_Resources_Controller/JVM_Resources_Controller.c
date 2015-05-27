#include"control_fun.h"
#include <windows.h>


JNIEXPORT jint JNICALL intMethod
(JNIEnv *a, jobject b, jint c){
	printf("test ssss%d", c);
	return c;
}


int Create_JVM_Management(){
	SetEnvironmentVariable("_ALT_JAVA_HOME_DIR", "C:\\cygwin64\\home\\Jane\\jdk\\openjdk9\\build\\windows-x86_64-normal-server-release\\images\\jdk");
	JavaVMOption options[1];
	options[0].optionString = "-Djava.class.path=./scala-library.jar;./scala-reflect.jar;.";
	JNI_Methods_Stack *js = alloc_JNI_Methods_Stack("JVM_Management_Library");
	js->add_Methods(js, (JNINativeMethod){ "intMethod", "(I)I", intMethod });
	printf("%s", js->Methods[0]->name);

	Create_VM(options, "JVM_Management");

}