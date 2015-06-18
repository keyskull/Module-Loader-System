package cn.MLS.JVM
import com.sun.tools.attach.VirtualMachine

class hid{
  
}

object hid {
import java.lang.management.ManagementFactory
  def get_hid(vm :VirtualMachine):Int=return vm.provider().hashCode()
}