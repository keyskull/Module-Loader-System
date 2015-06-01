#include <stdlib.h> 

#include"control_fun.h"
// ��������PATH��windows�º�linux�µķָ������


JNIEXPORT jint JNICALL intMethod
(JNIEnv *a, jobject b, jint c){
	printf("test ssss%d", c);
	return c;
}

typedef struct VM_JNI_Stack{	

	JNI_Methods_Stack **(*Get_JNI_Onload_Methods_Stack)(JavaVM *);
}VM_JNI_Stack;



static JNI_Methods_Stack *jni_methods_stack=NULL;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved) //����JNI_OnLoad�����������밴�������ķ�ʽ����
{
	if (jni_methods_stack == NULL)return JNI_VERSION_1_8;
	JNIEnv* env = NULL; //ע��ʱ��JNIEnv��ʵ�ֵģ����Ա������Ȼ�ȡ��
	jint result = -1;
	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_8) != JNI_OK)return -1; //��JavaVM��ȡJNIEnv��һ��ʹ��1.4�İ汾

	jclass cls = (*env)->FindClass(env, jni_methods_stack->ClassName); //��������ҵ�Ҫע����࣬ǰ����������Ѿ����ص�java������С� ����˵������̬�����native��������֮�䣬û���κζ�Ӧ��ϵ��
	if (cls == NULL){
		printf("cannot get class:%s\n", jni_methods_stack->ClassName);
		return -1;
	}
	if ((*env)->RegisterNatives(env, cls, jni_methods_stack->Methods,jni_methods_stack->len) != JNI_OK){ //������ǹؼ��ˣ��ѱ��غ�����һ��java�෽����������������֮ǰ�Ƿ��������һ�ɰ�֮ǰ���滻����
		printf("register native method failed!\n");
		return -1;
	}

	return JNI_VERSION_1_8; //�������Ҫ�����뷵�ذ汾��������ػ�ʧ�ܡ�
}

int Create_VM(JavaVMOption options[], char * Main_ClassName){

	JNIEnv *env;
	JavaVM *jvm;
	JavaVMInitArgs vm_args;
	long status;
	jclass cls;
	jmethodID mid;
	jfieldID fid;
	jobject obj;

	memset(&vm_args, 0, sizeof(vm_args));
	vm_args.version = JNI_VERSION_1_8;
	vm_args.nOptions = 1;
	vm_args.options = options;

	// ���������  
	status = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);

	if (status != JNI_ERR){
		// �Ȼ��class����  
		cls = (*env)->FindClass(env, Main_ClassName);
		if (cls != 0){
			// ��ȡ����ID, ͨ����������ǩ��, ���þ�̬����  
			mid = (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
			if (mid != 0){
				(*env)->CallStaticObjectMethod(env, cls, mid);
				JNI_OnLoad(jvm, NULL);
				(*env)->CallStaticObjectMethod(env, cls, mid);
			}
		}
		jthrowable throws = (*env)->ExceptionOccurred(env);
		if (throws != NULL)(*env)->ExceptionDescribe(env);
		(*jvm)->DestroyJavaVM(jvm);
		system("pause");
		return 0;
	}
	else
	{
		printf("JVM Created failed!\n");
		system("pause");
		return -1;

	}

}