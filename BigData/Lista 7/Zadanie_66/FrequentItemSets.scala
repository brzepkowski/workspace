object FrequentItemSets {

  def dictionary(T:Array[Set[String]]) = {
    println("T: " + T.toList + "\n")
    var S2I = T.reduceLeft(_.union(_)).toList.zipWithIndex.toMap
    println("S2I: " + S2I + "\n")
    var I2S = S2I.map(_.swap)
    println("I2S: " + I2S + "\n")
  }

  def main(args:Array[String]):Unit = {
    var T:Array[Set[String]] = new Array[Set[String]](5)
    T(0) = Set("apple", "peach")
    T(1) = Set("orange", "banana")
    T(2) = Set("pineapple")
    T(3) = Set("blackberry", "watermelon", "strawberry")
    T(4) = Set("mandarin", "orange", "banana")
    dictionary(T)
  }
}
