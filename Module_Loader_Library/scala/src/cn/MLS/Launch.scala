/**
 *
 */
package cn.MLS
import cn.MLS.JVM._
import scala.collection.JavaConverters._
import java.lang.Thread
/**
 * @author Cullen_Lee
 * @Create_date 2015-5-2 02:02:24
 *
 */



object Launch {
   def init():Unit={
     VM.Get_Local_VM_List.foreach{
       i =>
         println(i.Get_PID+" "+i.is_Online()+" " +" " +i.Type_name)
        if( i.isInstanceOf[Local_VM])
          new com.window.MemoryMonitor(i.asInstanceOf[Local_VM].MemoryPoolMXBeans.asJava).run()  
          
     }
     println("dsadsadasdfff")
   } 

  def main(args: Array[String]): Unit = {
    init

 
   }     
     
   
}