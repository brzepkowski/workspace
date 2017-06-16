import Jama._

object PageRank {


  def googleMatrix(alpha:Double, n:Int) = {

    var A = new Matrix(n, n)
    var B = new Matrix(n, n)

    for (i <- 0 until n) { // Columns
      for (j <- 0 until n) { // Rows
        if (j == i + 1) {
          A.set(j, i, 1.0)
        }
        if (i == n - 1) {
          A.set(j, n-1, 1/n.toDouble)
        }
      }
    }

    for (i <- 0 until n) { // Columns
      for (j <- 0 until n) { // Rows
        B.set(j, i, (1.0 - alpha)/n.toDouble)
      }
    }

    var G = A.times(alpha).plus(B)

    var x = new Matrix(n, 1)
    for (i <- 0 until n) {
      x.set(i, 0, 1.0 / n.toDouble)
    }

    for (i <- 0 until n + 1) {
      x = G.times(x)
    }


    println("A: ")
    A.getArray().foreach(row => println(row.mkString("\t")))
    println("G: ")
    G.getArray().foreach(row => println(row.mkString("\t")))
    println("x: ")
    x.getArray().foreach(row => println(row.mkString("\t")))
  }

  def main(args: Array[String]): Unit = {
    googleMatrix(0.85, 6)
  }
}
