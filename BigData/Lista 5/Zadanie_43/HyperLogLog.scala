import scala.io.Source
import java.io._
import scala.collection.mutable
import scala.util.hashing.MurmurHash3

object HyperLogLog {

  def hyperLogLog(b:Int, M:Iterator[String]):Unit = {
    var m = Math.pow(2, b).toInt
    var registers = new Array[Int](m) // Komórki tablicy są inicjowane zerami

    println("m: " + m)

    for (v <- M) {
      var parts = v.split(" ").toList
      // var x = parts(2).toInt.toBinaryString // Pomijamy haszowanie otrzymanych danych

      var x = MurmurHash3.stringHash(parts(1)).toBinaryString

      // Pozycja jedynki wiodącej = 16 - length(bits)
      for (i <- 0 to (32 - x.length)) {
        x = '0' + x
      }

      var buff = ""
      for (i <- 0 to (b-1)) {
        buff = buff + x(i)
      }
      var j = Integer.parseInt(buff, 2)
      var w = ""
      for (i <- b to (x.length - 1)) {
        w = w + x(i)
      }

      registers(j) = Math.max(registers(j), ro(w))
    }

    var Z = 1 / sumList(registers.toList)
    println("Z: " + Z)

    var alpha = calculateAlpha(m)
    var E = alpha * m * m * Z
    println("E = " + E)
  }

  def sumList(xs: List[Int]):Double = {
    var sum = 0.0
    for (i <- 0 to xs.length - 1) {
      if (xs(i) != 0) {
        sum += Math.pow(2, -xs(i))
      }
    }
    return sum
  }

  // Zwraca odgórnie obliczone wartości alpha
  def calculateAlpha(m:Int):Double = {
	   if (m == 16) {
       return 0.673
     } else if (m == 32) {
       return 0.697
     } else if (m == 64) {
       return 0.709
     } else {
       return 0.7213 / (1.0 + 1.079 / m.toFloat)
     }
   }

  // Założenie, że source i destination hosty zajmują nie więcej niż 16 bitów (na
  // podstawie rodzaju otrzymywanych danych)
  def ro(str:String):Int = {
    for (i <- 0 to (str.length - 1)) {
      if (str(i) == '1') {
        return i + 1
      }
    }
    return str.length
  }


	def main(args: Array[String]): Unit = {
    // Format danych: timestamp, (przenumerowany) source host, (przenumerowany) destination
    // host, source TCP port, destination TCP port, liczba bajtów danych(zero dla "pure-ack"pakietów)
		var M = Source.fromFile("lbl-pkt-4.tcp", "UTF-8").getLines()
    hyperLogLog(5, M)
	}
}
