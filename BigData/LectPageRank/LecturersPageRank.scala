import scala.collection.mutable
import Jama._

object LecturersPageRank {

  def main(args:Array[String]):Unit = {
    val lines = scala.io.Source.fromFile("LINKS.txt").getLines
    var allLinks:Set[Set[String]] = Set.empty
    var connections:collection.mutable.Map[String, Set[String]] = collection.mutable.Map.empty

    while (lines.hasNext) {
      var line = lines.next().split(" ")
      var linkName = line(0)
      var linksNumber = line(1).toInt

      var set:Set[String] = Set.empty
      set += linkName

      for (i <- 0 until linksNumber) {
        set += lines.next().split(" ")(1)
      }
      allLinks += set
      connections(linkName) = set
    }

    var links2I = allLinks.reduceLeft(_.union(_)).toList.zipWithIndex.toMap // Po tej linii już nie ptorzebujemy allLinks
    var connections2I:collection.mutable.Map[Int, Set[Int]] = collection.mutable.Map.empty

    connections.foreach({ x =>
      var set2I:Set[Int] = Set.empty
      x._2.foreach({ xs =>
        set2I += links2I(xs)
      })
      connections2I(links2I(x._1)) = set2I
    })

    // println(connections2I)
    var alpha = 0.85
    var n = links2I.size
    var A = new Matrix(n, n)
    var B = new Matrix(n, n)
    // (1.0 - alpha)/n.toDouble

    for (pair <- connections2I) {
      println(pair._1 + " -> " + pair._2)
      for (i <- pair._2) {
        A.set(i, pair._1, 1.0 / pair._2.size)
      }
    }

    var emptyColumns:collection.mutable.Set[Int] = collection.mutable.Set.empty

    for (i <- 0 until n) { // Columns
      var total = 0.0
      for (j <- 0 until n) { // Rows
        total += A.get(j, i)
      }
      if (total == 0.0) {
        emptyColumns += i
      }
      // println(i + " -> " + total)
    }

    // println("Empty: " + emptyColumns)

    for (i <- emptyColumns) {
      for (j <- 0 until n) { // Rows
        A.set(j, i, 1.0/n.toDouble)
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

    // println("A: ")
    // A.getArray().foreach(row => println(row.mkString("\t")))
    // println("G: ")
    // G.getArray().foreach(row => println(row.mkString("\t")))
    println("Page Rank: ")
    x.getArray().foreach(row => println(row.mkString("\t")))



  }
}
