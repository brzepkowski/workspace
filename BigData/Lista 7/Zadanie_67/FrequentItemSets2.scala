import scala.collection.mutable

object FrequentItemSets2 {

  def dictionary(T:Array[Set[String]]):Map[String, Int] = {
    var S2I = T.reduceLeft(_.union(_)).toList.zipWithIndex.toMap
    return S2I
  }

  def stringToInt(T:Array[Set[String]], TSI:Map[String, Int]):Array[Set[Int]] = {
    // println("T: " + T.toList)
    // println("TI: " + TSI)

    var T2:Array[Set[Int]] = new Array[Set[Int]](T.length)
    // T.foreach(x => x.foreach(xs => println(xs + " -> " + TI(xs))))
    for (i <- 0 until T.length) {
      var TS = T(i)
      var T2S:Set[Int] = Set()
      TS.foreach({ x =>
        T2S += TSI(x)
      })
      T2(i) = T2S
    }
    println(T2.toList)
    return T2
  }

  def RCRMap(TI:Array[Set[Int]], map:Map[String, Int]):collection.mutable.Map[Int,Int] = {
    var rcrMap: Set[(Int,Int)] = Set.empty
    map.foreach({ x =>
      var counter = 0
      TI.foreach({ xs =>
        if (xs.contains(x._2)) {
          counter += 1
        }
      })

      rcrMap += Tuple2(x._2, counter)
    })
    return collection.mutable.Map(rcrMap.toMap.toSeq: _*)
  }

  def main(args:Array[String]):Unit = {
    var T:Array[Set[String]] = new Array[Set[String]](5)
    T(0) = Set("apple", "peach")
    T(1) = Set("orange", "banana")
    T(2) = Set("pineapple", "banana")
    T(3) = Set("blackberry", "watermelon", "strawberry")
    T(4) = Set("mandarin", "orange", "banana")

    var n = T.length
    var TSI = dictionary(T)
    var TI = stringToInt(T, TSI)
    var rcrMap = RCRMap(TI, TSI)
    println("RCRMap: " + rcrMap)
    rcrMap = rcrMap.retain((k, v) => v.toDouble > 0.2*n.toDouble)
    println("Retained: " + rcrMap)
    var oftenObjects:Set[Int] = Set.empty
    rcrMap.foreach({ x =>
      oftenObjects += x._1
    })
    println("OftenObjects: " + oftenObjects)

  }
}
