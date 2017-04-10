import scala.collection.mutable.ListBuffer

var data = scala.io.Source.fromFile("TwoCollisions.csv").getLines.toList.map(_.split(",").map(_.toInt))
var found_pairs = new ListBuffer[(Int, Int)]

for (i <- data) {
	var persons_number = i(2)
	var persons_visits = data.filter(_(2) == i(2)) // Get all visits of this person 
	var collisions = new ListBuffer[Array[Int]]
	for (j <- persons_visits) {
		var other_visitors = data.filter(x => (x(0) == j(0) && x(1) == j(1) && x(2) != j(2)))
		if (other_visitors.length > 0) {
			for (k <- other_visitors) {
				collisions += k
			}
		}
	}

	for (x <- collisions) {
		if (collisions.filter(_(2) == x(2)).length >= 2) {
			if (!(found_pairs.exists{ a => a._1 == persons_number && a._2 == x(2)} || found_pairs.exists{ a => a._1 == x(2) && a._2 == persons_number})) {
				println("Visitors = " + persons_number + " | " + x(2))
				println("Hotel:  Date:")
				for (y <- collisions.filter(_(2) == x(2))) {
					println("    " + y(0) + "  : " + y(1))
				}
				found_pairs += new Tuple2(persons_number, x(2))
			}
		}
	}
}
