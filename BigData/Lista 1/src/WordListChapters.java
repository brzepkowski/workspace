import java.io.FileNotFoundException;
import java.io.UnsupportedEncodingException;
import java.util.HashMap;
import java.util.Map;

public class WordListChapters {
		
	public static void main(String[] args) throws FileNotFoundException, UnsupportedEncodingException {
		
		WordList wordListGenerator = new WordList();
		
		Map<Integer, HashMap<String, Integer>> chaptersLists = new HashMap<Integer, HashMap<String, Integer>>();
		Map<Integer, HashMap<String, Double>> chaptersTFIDFLists = new HashMap<Integer, HashMap<String, Double>>();
		
		for (int i = 1; i <= 12; i++) {
			// Generate list of words and appearances in every chapter
			Map<String, Integer> wordList = wordListGenerator.generateList("Zadanie_2/" + i);
			chaptersLists.put(i, (HashMap<String, Integer>) wordList);
		}
		
		for (Map.Entry<Integer, HashMap<String, Integer>> entry: chaptersLists.entrySet()) {
			wordListGenerator.deleteStopWords(entry.getValue());
		}
		
		for (int i = 1; i <= 12; i++) {
			// Calculate total amount of terms in chapter
			HashMap<String, Integer> wordList = chaptersLists.get(i);
			int totalWords = 0;
			for (Map.Entry<String, Integer> entry: wordList.entrySet()) {
				totalWords += entry.getValue();
			}
			
			wordListGenerator.saveListToFile("Zadanie_2/Words_Lists/List_"+i, 
												wordListGenerator.sortByValue(wordList));
			
			// Create map to store all future calculated TFIDF values
			HashMap<String, Double> wordsTFIDF = new HashMap<String, Double>();
			
			// Calculate TFIDF for each word in every chapter and save all values in new structure
			for (Map.Entry<String, Integer> entry: wordList.entrySet()) {
				String word = entry.getKey();
				
				double TFij = (double) entry.getValue() / totalWords;
				
				// Calculate IDFi				
				int documentsContainingWord = 0;
				for (Map.Entry<Integer, HashMap<String, Integer>> chapterListEntry: chaptersLists.entrySet()) {
					if (chapterListEntry.getValue().containsKey(word)) {
						documentsContainingWord++;
					}
				}
				// Below 12 = m - amount of documents in corpus
				double IDFi = Math.log((double) 12 / documentsContainingWord) / Math.log(2.0);
				//System.out.println(k + " - " + word + " - " + TFij + " - " 
				//						+ documentsContainingWord + " - " + IDFi);
				
				double TFIDFij = TFij * IDFi;
				
				wordsTFIDF.put(word, TFIDFij);
			}
			
			/*for (Map.Entry<String, Double> entry2: wordsTFIDF.entrySet()) {
				System.out.println(entry2.getKey() + " - " + entry2.getValue());
			}*/
			
			wordsTFIDF = (HashMap<String, Double>) wordListGenerator.sortByValue(wordsTFIDF);
			
			wordListGenerator.saveListToFileDouble("Zadanie_2/TFIDF_Lists/List_" + i, wordsTFIDF);

			System.out.println("Chapter " + i + " complete.");
			
			chaptersTFIDFLists.put(i, wordsTFIDF);
		}
			

	}
}
