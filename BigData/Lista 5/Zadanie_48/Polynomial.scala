import java.io._
import scala.util.Random
import hist.Histogram

object Polynomial {
  def generatePolynomial():Array[Int] = {
    val randomGenerator = new Random()
    var polynomial = new Array[Int](4)

    for (i <- 0 to 3) {
      polynomial(i) = randomGenerator.nextInt(100) % 11
    }

    return polynomial
  }

  def main(args: Array[String]): Unit = {
    var data = new Array[Double](100)

    var counter = 0
    for (i <- 0 to 99) {
      var polynomial = generatePolynomial()
      data(i) = polynomial(0)
      if (polynomial(0) == 2) {
        counter += 1
      }
    }
    println("|h_i(0) = 2|: " + counter)

    val values = List(0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0)
    val hist = new Histogram(data.toList, values)
    // println(hist.result)
    hist.print()

  }
}
