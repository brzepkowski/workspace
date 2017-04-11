import scala.io.Source
import java.io._
import scala.collection.mutable

object Jaccard {

	def jaccard(f1:String, f2:String, k:Integer) :Double = {
		var book1 = Source.fromFile(f1, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var book2 = Source.fromFile(f2, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var kGrams1 = book1.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet
		var kGrams2 = book2.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet

		return (kGrams1 & kGrams2).size.toDouble/((kGrams1 | kGrams2).size.toDouble)
	}

	def main(args: Array[String]): Unit = {
		println(jaccard(args(0), args(1), args(2).toInt))
	}
}

