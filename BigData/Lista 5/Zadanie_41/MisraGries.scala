import scala.collection.mutable

object MisraGries {

  def misraGries(stream:List[String], k:Int):scala.collection.mutable.Map[String,Int] = {
    var A = scala.collection.mutable.Map[String,Int]()
    var x = ""

    for (x <- stream) {
      if (A.contains(x)) {
        A.update(x, A(x) + 1)
      } else {
        A.put(x, 1)
      }
      if (A.size >= k) {
        // println("Przed: " + A)
        A.foreach { y =>
          if (y._2 - 1 == 0) {
            A.remove(y._1)
          } else {
            A.update(y._1, y._2 - 1)
          }
        }
        // println("Po:    " + A)
      }
    }
    return A
  }

  def main(args: Array[String]): Unit = {
    println(misraGries(List("ala", "ala", "ala", "ma", "ma", "ala", "kot"), 2))
  }
}
