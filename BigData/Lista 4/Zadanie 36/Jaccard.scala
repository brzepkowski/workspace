import scala.io.Source
import java.io._
import scala.collection.mutable

def jaccard(f1:String, f2:String, k:Integer) :Double = {
  var book1 = Source.fromFile(f1, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase //.split("\\s+")
  var book2 = Source.fromFile(f2, "UTF-8").mkString.replaceAll("""[\p{Punct}]""", "").replaceAll("—", "").replaceAll(" ", "").toLowerCase //.split("\\s+")
  val list1 = mutable.Set.empty[String]
  val list2 = mutable.Set.empty[String]

  //book1.iterator.sliding(k).withPartial(false).map(x=>x.mkString).toSet

  for (i <- Range(0, book1.length())) {
    if (i + k < book1.length()) {
      list1 += book1.substring(i, i + k)
    }
    //println(Book.substring(i, i+3))
  }
  for (i <- Range(0, book2.length())) {
    if (i + k < book2.length()) {
      list1 += book2.substring(i, i + k)
    }
    //println(Book.substring(i, i+3))
  }
  //println((list1 & list2).size().toDouble/((list1 | list2).size().toDouble()))
  println("123")
  println((list1 & list2).size.toDouble)
  println("321321")
  return 0.0
}

def main(args: Array[String]): Unit = {
  println("dwa")
  jaccard("Ostatnie_zyczenie.txt", "Ostatnie_zyczenie.txt", 3)
  println("dwafe")
}
/*
var Groupped = Filtered.groupBy(x=>x)

var Reduced = Groupped.mapValues(x=>x.length)

var Final = Reduced.toSeq.sortWith((x,y)=>x._2>y._2)

val writer = new PrintWriter(new File("List.txt" ))

for (pair <- Final) {
  writer.write(pair._2 + " " + pair._1 + "\n")
}
writer.close()*/
