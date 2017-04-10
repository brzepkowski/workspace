import scala.io.Source
import java.io._

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

def TextReducer(keyval:Seq[(String,String)],fn:String):Unit ={
	val printer  = new PrintWriter(new File(fn),"UTF-8")
	
	val groupped = keyval.groupBy(_._1)
	val reduced  = groupped.mapValues(_.size)
	// 	Ta część - sortowanie - jest robiona dla wygody; z reguły nie jest 	wykonywana przez MapReduce
	val sorted   = reduced.toSeq.sortWith(_._2>_._2)
	
	for (x<-sorted){
		printer.write(x._1.toString+","+x._2.toString+"\n")
	}
	printer.close
}
