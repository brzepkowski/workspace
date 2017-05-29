object BoyerMoore {

  def boyerMoore(stream:List[String]):String = {
    var counter = 0
    var m = ""
    var x = ""

    // add(x)
    for (x <- stream) {
      if (counter == 0) {
        m = x
      }
      if (x == m) {
        counter += 1
      } else {
        counter -= 1
      }
    }
    return m
  }

  def main(args: Array[String]): Unit = {
    // Dla tego przykladu dziala
    println(boyerMoore(List("ala", "ala", "ala", "ala", "ma", "ma", "ma", "ma", "ala", "kot")))
    // Ciekawy przypadek - zwraca "kot"
    println(boyerMoore(List("ala", "ala", "ala", "ma", "ma", "ma", "ma", "ala", "kot")))
  }
}
