import scala.io.Source
import java.io._
import scala.collection.mutable
import scala.util.Random

object CosineSignatures {

	val STOPWORDS = Array("a", "aby", "ach", "acz", "aczkolwiek", "aj", "albo", "ale", "ależ", "ani", "aż", "bardziej", "bardzo", "bo", "bowiem", "by", "byli", "bynajmniej", "być", "był", "była", "było", "były", "będzie", "będą", "cali", "cała", "cały", "ci", "cię", "ciebie", "co", "cokolwiek", "coś", "czasami", "czasem", "czemu", "czy", "czyli", "daleko", "dla", "dlaczego", "dlatego", "do", "dobrze", "dokąd", "dość", "dużo", "dwa", "dwaj", "dwie", "dwoje", "dziś", "dzisiaj", "gdy", "gdyby", "gdyż", "gdzie", "gdziekolwiek", "gdzieś", "i", "ich", "ile", "im", "inna", "inne", "inny", "innych", "iż", "ja", "ją", "jak", "jakaś", "jakby", "jaki", "jakichś", "jakie", "jakiś", "jakiż", "jakkolwiek", "jako", "jakoś", "je", "jeden", "jedna", "jedno", "jednak", "jednakże", "jego", "jej", "jemu", "jest", "jestem", "jeszcze", "jeśli", "jeżeli", "już", "ją", "każdy", "kiedy", "kilka", "kimś", "kto", "ktokolwiek", "ktoś", "która", "które", "którego", "której", "który", "których", "którym", "którzy", "ku", "lat", "lecz", "lub", "ma", "mają", "mało", "mam", "mi", "mimo", "między", "mną", "mnie", "mogą", "moi", "moim", "moja", "moje", "może", "możliwe", "można", "mój", "mu", "musi", "my", "na", "nad", "nam", "nami", "nas", "nasi", "nasz", "nasza", "nasze", "naszego", "naszych", "natomiast", "natychmiast", "nawet", "nią", "nic", "nich", "nie", "niech", "niego", "niej", "niemu", "nigdy", "nim", "nimi", "niż", "no", "o", "obok", "od", "około", "on", "ona", "one", "oni", "ono", "oraz", "oto", "owszem", "pan", "pana", "pani", "po", "pod", "podczas", "pomimo", "ponad", "ponieważ", "powinien", "powinna", "powinni", "powinno", "poza", "prawie", "przecież", "przed", "przede", "przedtem", "przez", "przy", "roku", "również", "sama", "są", "się", "skąd", "sobie", "sobą", "sposób", "swoje", "ta", "tak", "taka", "taki", "takie", "także", "tam", "te", "tego", "tej", "temu", "ten", "teraz", "też", "to", "tobą", "tobie", "toteż", "trzeba", "tu", "tutaj", "twoi", "twoim", "twoja", "twoje", "twym", "twój", "ty", "tych", "tylko", "tym", "u", "w", "wam", "wami", "was", "wasz", "wasza", "wasze", "we", "według", "wiele", "wielu", "więc", "więcej", "wszyscy", "wszystkich", "wszystkie", "wszystkim", "wszystko", "wtedy", "wy", "właśnie", "z", "za", "zapewne", "zawsze", "ze", "zł", "znowu", "znów", "został", "żaden", "żadna", "żadne", "żadnych", "że", "żeby")

	def notInStopWords(w:String):Boolean = {
		 ! STOPWORDS.contains(w)
	}

	def TextMapper(fn:String):Seq[(String,String)] = {
		val bufor  = Source.fromFile(fn,"UTF-8")
		var keyval = collection.mutable.ListBuffer.empty[(String,String)]
		var slowa  : Array[String] = Array()

		var rob    = ""
		// Wczytujemy kolejne linie, aby to bardziej upodobnić prawdziwego Mappera
		for (line <- bufor.getLines()) {
			slowa = line.mkString.split("\\s+")
			for (slowo <- slowa if notInStopWords(slowo)){
				rob = slowo.toLowerCase().replaceAll("[,.!:?*;»…()«]","")
				if (rob.length>1)
					keyval += Tuple2(rob,"1")
			}
		}
		bufor.close
		keyval
	}

	def TextReducer(keyval:Seq[(String,String)]):Map[String,Int] ={
		val groupped = keyval.groupBy(_._1)
		val reduced  = groupped.mapValues(_.size)
		reduced
	}

	//--------Reuben Sutton - 2012---------------
	def cosineSimilarity(x: Array[Double], y: Array[Double]): Double = {
		require(x.size == y.size)
		dotProduct(x, y)/(magnitude(x) * magnitude(y))
	}

	/*
	 * Return the dot product of the 2 arrays
	 * e.g. (a[0]*b[0])+(a[1]*a[2])
	 */
	def dotProduct(x: Array[Double], y: Array[Double]): Double = {
		(for((a, b) <- x zip y) yield a * b) sum
	}

	/*
	 * Return the magnitude of an array
	 * We multiply each element, sum it, then square root the result.
	 */
	def magnitude(x: Array[Int]): Double = {
		math.sqrt(x map(i => i*i) sum)
	}
	//------------------------------------------
	def magnitude(x: Array[Double]): Double = {
		math.sqrt(x map(i => i*i) sum)
	}


	def cosineSignatures(f1:String, f2:String) :Unit = {
		var words1 = TextReducer(TextMapper(f1)).toSet
		var words2 = TextReducer(TextMapper(f2)).toSet
		val wordsSum = (words1 | words2)
		var words1Map = words1.toMap
		var words2Map = words2.toMap

		var vector1 = collection.mutable.ListBuffer.empty[Int]
		var vector2 = collection.mutable.ListBuffer.empty[Int]
		wordsSum.foreach{ i => vector1 += words1Map.get(i._1).getOrElse(0)}
		wordsSum.foreach{ i => vector2 += words2Map.get(i._1).getOrElse(0)}

		var magnitude1 = magnitude(vector1.toArray)
		var magnitude2 = magnitude(vector2.toArray)
		var arr1 = vector1.toArray.map(i => i / magnitude1)
		var arr2 = vector2.toArray.map(i => i / magnitude2)
		var signature = collection.mutable.ListBuffer.empty[(Double, Double)]

		for (i <- Range(0, 1024)) {
			var randomVector = Seq.fill(vector1.size)((Random.nextDouble*2)-1).toArray
			signature += Tuple2(cosineSimilarity(arr1, randomVector), cosineSimilarity(arr2, randomVector))
			}

		val writer = new PrintWriter(new File("signature.txt" ))

		for (pair <- signature) {
			var x1 = 1
			var x2 = 1
			if (pair._1 < 0) x1 = -1
			if (pair._2 < 0) x2 = -1
			writer.write(x1 + " | " + x2 + "\n")
		}
		writer.close()
	}

	def main(args: Array[String]): Unit = {
		cosineSignatures(args(0), args(1))
	}
}
