package cn.MLS.JVM
import com.sun.tools.attach.VirtualMachineDescriptor


abstract class Connect_Function {
  
    private[Connect_Function] val connection_time: Long=System.currentTimeMillis()
    def Get_Connection_time()=this.connection_time
    def Connection():VM
}

final class Connected(connect:Connect_Function){
  private val Connected_time=System.currentTimeMillis()
  def get_Connected_time=this.Connected_time 
}

class Local_Connect(vmd:VirtualMachineDescriptor) extends Connect_Function{
  def Connection():VM=return null

}
