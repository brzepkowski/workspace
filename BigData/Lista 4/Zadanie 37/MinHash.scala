import scala.io.Source
import java.io._
import scala.collection.mutable
import scala.util.hashing.MurmurHash3 

object MinHash {

	def jaccard(f1:String, f2:String, k:Integer) :Double = {
		var book1 = Source.fromFile(f1, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var book2 = Source.fromFile(f2, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var kGrams1 = book1.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet
		var kGrams2 = book2.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet

		return (kGrams1 & kGrams2).size.toDouble/((kGrams1 | kGrams2).size.toDouble)
	}

//-----------------------------------------------------------------

	def minHash(f1:String, f2:String, k:Integer, h:Integer) :Unit = {
		var book1 = Source.fromFile(f1, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var book2 = Source.fromFile(f2, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase
		var kGrams1 = book1.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet
		var kGrams2 = book2.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet

		var hashes1 = mutable.Set.empty[Int]

		var seed = 1234567890
		for (i <- Range(0, h)) {
			for (k <- kGrams1) {
				hashes1 += MurmurHash3.stringHash(k, seed + i)
			}
		}

		println(hashes1.min)

	}

	def main(args: Array[String]): Unit = {
		//println(jaccard(args(0), args(1), args(2).toInt))
		minHash(args(0), args(1), args(2).toInt, args(3).toInt)
	}
}
