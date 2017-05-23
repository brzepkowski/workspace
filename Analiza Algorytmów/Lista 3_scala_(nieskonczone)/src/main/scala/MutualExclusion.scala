import scala.util.Random
import scala.collection.mutable

class Box() {
  var value = 0
}

object MutualExclusion {
  var checkedConfigurations = mutable.Set.empty[(Array[Int], Int)] // Set zawierający wszystkie konfiguracje, które już zostały sprawdzone

  def checkWhichApplicableForCS(processors: Array[Int], n: Int): mutable.Set[Int] = {
    val criticalSectionApplicable = mutable.Set.empty[Int] // Zbiór zawierający indeksy procesorów do sekcji krytycznej

    if (processors(0) == processors(n-1)) { // Critical section
      criticalSectionApplicable += 0
    }

    for (i <- 1 to n-1) {
      if (processors(i) != processors(i-1)) { // Critical section
        criticalSectionApplicable += i
      }
    }

    for (i <- 0 to n-1) {
      println(i + " || " + processors(i) + " | " + processors((((i - 1 % n) + n) % n)))
    }
    println("---CRITICAL SECTION---")
      for (x <- criticalSectionApplicable) {
        println(x + " || " + processors(x) + " | " + processors((((x - 1 % n) + n) % n)))
      }
    println("----------------------")
    return criticalSectionApplicable
  }

  class MutualExclusionThread(processors: Array[Int], processorsApplicableForCS: Array[Int], processorInCS: Int, n: Int, maxSteps: Box) extends Runnable {
    def run() {
      println("Wątek: " + processorInCS)
      println("PRZED:")
      checkWhichApplicableForCS(processors, n)

      if (processorInCS == 0) {
        var registry = processors(processorInCS)
        processors(processorInCS) = (registry + 1) % (n + 1)
      } else {
        var prevRegistry = processors(processorInCS - 1)
        processors(processorInCS) = prevRegistry
      }
      println("PO:")
      val criticalSectionApplicable = checkWhichApplicableForCS(processors, n)

      var maxSteps2 = new Box()
      //Thread.sleep(3000)
      if (criticalSectionApplicable.size > 1 && !checkedConfigurations.toMap.contains(processors)) {
        for (i <- criticalSectionApplicable) {
          (new MutualExclusionThread(processors.clone, criticalSectionApplicable.clone.toArray, i, n, maxSteps2)).run()
        }
      }

      maxSteps2.value += 1

      synchronized {
        if (maxSteps2.value > maxSteps.value) {
          maxSteps.value = maxSteps2.value
        }

        checkedConfigurations += Tuple2(processors, maxSteps2.value)
      }
    }
  }

  def main(args: Array[String]): Unit = {
    var n = args(0).toInt
    val random = new Random()
    var processors = Array.fill(n)(random.nextInt(n)) // Stworzenie tablicy ze wsystkimi procesorami (długości n)

    //checkedConfigurations += processors

    // Sprawdzenie, które procesory mogą wejść do sekcji krytycznej
    val criticalSectionApplicable = checkWhichApplicableForCS(processors, n)


    var maxSteps = new Box()

    // Główna pętla algorytmu
    if (criticalSectionApplicable.size > 1 && !checkedConfigurations.toMap.contains(processors)) {
      for (i <- criticalSectionApplicable) {
        println("--------OUT-----------------")
        (new MutualExclusionThread(processors.clone, criticalSectionApplicable.clone.toArray, i, n, maxSteps)).run()
        println("<<<<<<<<<<<<>>>>>>>>>>>>>>>>")
      }
    }

    checkedConfigurations += Tuple2(processors, maxSteps.value + 1)

    /*for (i <- checkedConfigurations) {
      for (j <- i._1) {
        print(j + ", ")
      }
      println(" -> " + i._2)
    }*/
    println("MAX = " + checkedConfigurations.maxBy(_._2)._2)
  }
}
