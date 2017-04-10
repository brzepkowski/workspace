import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.stream.Collectors;

public class WordList {
	
	public Map<String, Integer> stopWords;
	
	public WordList() throws FileNotFoundException {
		stopWords = generateList("Stop_words.txt");
	}
	
	// http://stackoverflow.com/questions/109383/sort-a-mapkey-value-by-values-java
	public <K, V extends Comparable<? super V>> Map<K, V> sortByValue(Map<K, V> map) {
	    return map.entrySet()
	              .stream()
	              .sorted(Map.Entry.comparingByValue(Collections.reverseOrder()))
	              .collect(Collectors.toMap(
	                Map.Entry::getKey, 
	                Map.Entry::getValue, 
	                (e1, e2) -> e1, 
	                LinkedHashMap::new
	              ));
	}
	
	public Map<String, Integer> generateList(String pathToFile) throws FileNotFoundException {
		File file = new File(pathToFile);
		Scanner scanner = new Scanner(file);
		Map<String, Integer> wordList = new HashMap<String, Integer>();
		
		while (scanner.hasNext()) {
			String word = scanner.next();
			word = word.replaceAll("[-+.^:,_?!; —\n]", "").toLowerCase();
			if (!wordList.containsKey(word)) {
				wordList.put(word, 1);
			} else {
				wordList.put(word, wordList.get(word) + 1);
			}
		}
		wordList.remove("");
		
		return wordList;
	}
	
	public void deleteStopWords(Map<String, Integer> map) {
		// Remove all stop words
		for (Map.Entry<String, Integer> stopWord: stopWords.entrySet()) {
			map.remove(stopWord.getKey());
		}
	}
	
	public void saveListToFile(String pathToFile, Map<String, Integer> map) throws FileNotFoundException, UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter(pathToFile, "UTF-8");

		for (Map.Entry<String, Integer> entry: map.entrySet()) {
			writer.println(entry.getValue() + " " + entry.getKey());
		}
		writer.close();
	}
	
	public void saveListToFileDouble(String pathToFile, Map<String, Double> map) throws FileNotFoundException, UnsupportedEncodingException {
		PrintWriter writer = new PrintWriter(pathToFile, "UTF-8");

		for (Map.Entry<String, Double> entry: map.entrySet()) {
			writer.println(entry.getValue() + " " + entry.getKey());
		}
		writer.close();
	}
	
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		WordList wordListGenerator = new WordList();
		
		// Generate list of words and appearances
		Map<String, Integer> wordList = wordListGenerator.generateList("Zadanie_1/Ostatnie_zyczenie.txt");
		
		// Remove all stop words
		wordListGenerator.deleteStopWords(wordList);
			
		// Sort the word list
		wordList = wordListGenerator.sortByValue(wordList);
		
		// Save list to file
		wordListGenerator.saveListToFile("Zadanie_1/List.txt", wordList);
	}
}
