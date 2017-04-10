import scala.util.Random
import scala.collection.mutable

object BFS {

  def move(c:Array[Int]) : Array[Int] = {
    var v = Array(8, 5, 3)
    var copy = c.clone
		var pitchers = Random.shuffle(Array(0, 1, 2).toList).take(2)
		var x = pitchers(0)
		var y = pitchers(1)
		var sum = c(x) + c(y)
		if (sum <= v(y)) {
			copy(x) = 0
			copy(y) = sum
		} else {
			copy(x) = sum - v(y)
			copy(y) = v(y)
		}
    copy
  }

  def main(args: Array[String]): Unit = {
		var arr = Array(8, 0, 0)
		val moves1 = mutable.Set.empty[String]
		val moves2 = mutable.Set.empty[String]
		moves1 += arr.toList.toString

		while (arr.deep != Array(4, 4, 0).deep) {
			println(arr.toList.toString)
			for (i <- 1 to 100) { // Parody of brute-force...
				val new_arr = move(arr)
				if (!moves2(new_arr.toList.toString)) {
					moves2 += new_arr.toList.toString
				}
			}
			for (x <- moves2) {
				if (!moves1(x)) {
					arr =  x.replaceAll( "[^\\d]", "" ).toArray.map(_.toInt - 48)
					moves1 += x
					moves2 -= x
				}
			}
		}
		println(arr.toList.toString)
  }
}
