package cn.MLS.JVM


import scala.collection.JavaConverters._
import com.sun.tools.attach.VirtualMachine
import java.util.Properties
import javax.management.MBeanServerConnection
import java.lang.management.ManagementFactory
import javax.management.ObjectInstance
import javax.management.ObjectName
import java.lang.management.RuntimeMXBean
import java.lang.management.MemoryPoolMXBean
import javax.management.remote.JMXServiceURL
import javax.management.remote.JMXConnectorFactory
import javax.management.remote.JMXConnector

 sealed abstract class VM_status 
 case object Running extends VM_status
 case object Pausing  extends VM_status
 case object Disconnect extends VM_status
 case object UnKnow extends VM_status
 
abstract class VM extends VM_Type{
  @native def Get_Output_Stream
  @native def Get_Input_Stream
  @native def Get_Error_Stream
  def is_Online():Boolean
  def Get_PID():Int
  def Get_HID():Int
  def Get_VM_HashCode():Int
  def Get_host_name():String
  def Get_Mbean_list:List[String]
  def flush():Boolean
}

class Local_VM(vm: VirtualMachine) extends VM{
  flush()
  private lazy val id:Int=vm.id.toInt
  private lazy val host_name:String=vm.provider().name
  private lazy val vm_name:String=vm.toString()
  private lazy val vm_hashcode :Int=vm.hashCode()
  private lazy val host_hashcode: Int=vm.provider().hashCode()
  private lazy val Mbean_Server:MBeanServerConnection=Local_VM.Get_Loval_VM_MBeansServer(vm)
  private lazy val Agent_Properties=vm.getAgentProperties
  private lazy val System_Properties=vm.getSystemProperties
  private lazy val Mbean_list:List[String]= Mbean_Server.queryNames(null,null).asScala.map((x:ObjectName) => x.toString).toList
  lazy val Runtime_Mbean:RuntimeMXBean=ManagementFactory.newPlatformMXBeanProxy(Mbean_Server, ManagementFactory.RUNTIME_MXBEAN_NAME,classOf[RuntimeMXBean]) 
  lazy val MemoryPoolMXBeans:List[MemoryPoolMXBean]=Mbean_list.filter { 
    x => x.startsWith(ManagementFactory.MEMORY_POOL_MXBEAN_DOMAIN_TYPE)}.map {
    y => ManagementFactory.newPlatformMXBeanProxy(Mbean_Server,y,classOf[MemoryPoolMXBean])
    }.toList
  private var is_online :Boolean = false
  private var status:VM_status=UnKnow
  private var uptime:Long=Runtime_Mbean.getUptime
  def Get_SystemProperties():Properties=return this.Agent_Properties
  def Get_AgentProperties():Properties=return this.System_Properties
  def Get_Mbean_list:List[String]=this.Mbean_list
  def is_Online():Boolean=this.is_online
  def Get_PID():Int=return this.id
  def Get_HID():Int=return this.host_hashcode
  def Get_VM_HashCode():Int=return this.vm_hashcode
  def Get_host_name():String=return this.host_name
  Mbean_Server
  def flush():Boolean={ //刷新虚拟机状态
   try {
    val time:Long=Runtime_Mbean.getUptime
    if(uptime < time){
      uptime =time 
      this.is_online=true
    }
   }catch{  case _:Exception => this.is_online=false }
   return true
  }
  def VM_detach():Boolean={
   vm.detach()
   this.status=Disconnect
   return true
  }
}

class Remote_VM(connector: JMXConnector) extends VM{
  flush()
  lazy val Runtime_Mbean:RuntimeMXBean=ManagementFactory.newPlatformMXBeanProxy(Mbean_Server, ManagementFactory.RUNTIME_MXBEAN_NAME,classOf[RuntimeMXBean])
  private lazy val id:Int=Runtime_Mbean.getName.split("@")(0).toInt
  private lazy val Mbean_Server:MBeanServerConnection=connector.getMBeanServerConnection()
  private lazy val Mbean_list:List[String] = Mbean_Server.queryNames(null,null).asScala.map((x:ObjectName) => x.toString).toList
  private lazy val host_name:String=Mbean_Server.toString()
  private lazy val vm_name:String=Mbean_Server.toString()
  private lazy val vm_hashcode :Int=Mbean_Server.hashCode()
  private lazy val host_hashcode: Int=Mbean_Server.hashCode()
  private var status:VM_status=UnKnow
  private var is_online :Boolean = false
  def is_Online():Boolean=this.is_online
  def Get_PID():Int=return this.id
  def Get_HID():Int=return this.host_hashcode
  def Get_VM_HashCode():Int=return this.vm_hashcode
  def Get_host_name():String=return this.host_name
  def Get_Mbean_list:List[String]=this.Mbean_list

  def flush():Boolean={ //刷新虚拟机状态
   try println(Runtime_Mbean.getName)
   catch{  case _:Exception => this.is_online=false;return true }
   this.is_online=true
   return true
  }
}

object VM{
  private var VM_list:Set[VM]=Set()
  val my_id=ManagementFactory.getRuntimeMXBean.getName.split("@")(0)
  @native def Create_VM():Boolean
  @native def Get_VM_RunTime_Type(hid :Int,pid:Int):VM_RunTime_Type
  Init_localVM()
  def Get_Local_VM_List():List[VM]=return VM_list.toList
  def mark_VM_msg(vm: VM):Unit= VM_list += vm
  def Flush_localVM():List[Int]=VirtualMachine.list.asScala.map{ x => x.id().toInt}.toList
  def Init():Unit=VM_list += _Create_VM(VirtualMachine.attach(my_id),classOf[JVM_Management_System]) 
  
  def Init_localVM()=VirtualMachine.list.asScala.foreach{ i => 
    val vm=VirtualMachine.attach(i)
    if(i.id ==my_id) {
      val vms=  _Create_VM(vm,classOf[JVM_Management_System]) 
      VM_list += vms
    }
    else VM_list += _Create_VM(vm,classOf[VM_Application])
  }
  
  def _Create_VM(any: Any,traits: Class[_<:VM_RunTime_Type]):VM={
      def Local_match(vm:VirtualMachine):VM=traits match{
       case _:Class[JVM_Management_System]=>new Local_VM(vm) with JVM_Management_System
       case _:Class[VM_Application]=>new Local_VM(vm) with VM_Application
       case _ => new Local_VM(vm)
     }
   def Remote_match(jmx:JMXConnector):VM=traits match{
       case _:Class[JVM_Management_System]=>new Remote_VM(jmx) with JVM_Management_System
       case _:Class[VM_Application]=>new Remote_VM(jmx)with VM_Application
       case _ => new Remote_VM(jmx)
     }
    any match{
     case vm:VirtualMachine =>  return Local_match(vm)
     case jmx:JMXConnector=> return Remote_match(jmx)
     case _ => println("no");return null
    }
  }
}


object Local_VM{
  import java.io.File
   private[Local_VM] def Get_Loval_VM_MBeansServer(vm:VirtualMachine):MBeanServerConnection={
     if(vm.id==VM.my_id)return java.lang.management.ManagementFactory.getPlatformMBeanServer
     else{
     val javaHome = vm.getSystemProperties().getProperty("java.home");  
     val jmxAgent = javaHome + File.separator + "lib" + File.separator + "management-agent.jar"  
     vm.loadAgent(jmxAgent, "com.sun.management.jmxremote");  
     // 获得连接地址  
     val properties = vm.getAgentProperties();  
     val address = properties.get("com.sun.management.jmxremote.localConnectorAddress")
     // 通过jxm address来获取RuntimeMXBean对象，从而得到虚拟机运行时相关信息  
     val url = new JMXServiceURL(address.toString); 
     val connector = JMXConnectorFactory.connect(url)
     return connector.getMBeanServerConnection()
     }
   }
}

