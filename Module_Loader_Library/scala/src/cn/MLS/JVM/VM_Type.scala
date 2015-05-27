package cn.MLS.JVM

 abstract class VM_Type {
   def Type_name():String=return "Unknow"
   def Get_VM_Function():Set[Any]=Set()

}
trait VM_RunTime_Type extends VM_Type

trait JVM_Management_System extends VM_RunTime_Type{
  override def Type_name():String=return "JVM_Management_System"
  override def Get_VM_Function():Set[Any]=Set()
  
}

trait VM_Application extends VM_RunTime_Type{
   override def Type_name():String=return "VM_Application"
   override def Get_VM_Function():Set[Any]=Set()
}

trait Can_Contorl_VM {
  def Pause_VM():Boolean
  def Continue_VM():Boolean
  def Stop_VM():Boolean
}