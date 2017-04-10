import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.RefineryUtilities;
import org.jfree.data.xy.IntervalXYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class Main {

	public static Random rand = new Random();
	
	public static int election_1(int n) {
		String status = "NULL";
		int slotsCounter = 1;
		while (!status.equals("SINGLE")) {
			status = "NULL";
			for (int i = 0; i < n; i++) {
				double broadcast = rand.nextDouble();
				if (broadcast < ((double) 1 / n)) {
					if (status.equals("NULL"))
						status = "SINGLE";
					else if (status.equals("SINGLE")) {
						status = "CONFLICT";
					}
				}
			}
			slotsCounter++;
		}
		return (slotsCounter - 1);
	}
	
	// Returns number of slots
	public static int election_2(int u, int n) {
		String status = "NULL";
		double l = Math.ceil(Math.log(u) / Math.log(2));
		int slotCounter = 1;
		while (!status.equals("SINGLE")) {
			for (int i = 1; i <= l; i++) {
				status = "NULL";
				for (int j = 0; j < n; j++) {
					double broadcast = rand.nextDouble();
					if (broadcast < ((double) 1 / Math.pow(2.0, i))) {
						if (status.equals("NULL"))
							status = "SINGLE";
						else if (status.equals("SINGLE")) {
							status = "CONFLICT";
						}
					}
				}
				if (status == "SINGLE") return slotCounter;
				slotCounter++;
			}
		}
		return (slotCounter - 1);
	}
	
	// Returns number of rounds
	public static int election_2_2(int u, int n) {
		String status = "NULL";
		double l = Math.ceil(Math.log(u) / Math.log(2));
		int roundCounter = 1;
		while (!status.equals("SINGLE")) {
			for (int i = 1; i <= l; i++) {
				status = "NULL";
				for (int j = 0; j < n; j++) {
					double broadcast = rand.nextDouble();
					if (broadcast < ((double) 1 / Math.pow(2.0, i))) {
						if (status.equals("NULL"))
							status = "SINGLE";
						else if (status.equals("SINGLE")) {
							status = "CONFLICT";
						}
					}
				}
				if (status == "SINGLE") break;
			}
			roundCounter++;
		}
		return (roundCounter - 1);
	}
	
	public static void generateHistogram_1(int n, int numberOfExperiments) {
		Map<Integer, Integer> slots = new HashMap<Integer, Integer>();
		List<Integer> eachSlot = new ArrayList<Integer>();
		int totalSlots = 0;
		
		for (int i = 0; i < numberOfExperiments; i++) {
			int slot = election_1(n);
			eachSlot.add(slot);
			totalSlots += slot;
			if (slots.containsKey(slot)) {
				slots.put(slot, slots.get(slot) + 1);
			} else {
				slots.put(slot, 1);
			}
		}
		
		double expectedValue = (double) totalSlots / numberOfExperiments;
		double variance = 0;
		
		for (Integer entry: eachSlot) {
			variance += Math.pow((double) entry - expectedValue, 2.0);
		}
		variance = variance / numberOfExperiments;
		
		System.out.println("Wartość oczekiwana = " + expectedValue + ", wariancja = " + variance);
		
		Histogram demo = new Histogram("Histogram - Known n", slots);
        demo.pack();
        RefineryUtilities.centerFrameOnScreen(demo);
        demo.setVisible(true);
	}

	
	// This version for every n from 2 to u runs number of tests = numberOfExperiments
	public static void generateHistogram_2(int u, int numberOfExperiments) {
		Map<Integer, Integer> rounds1 = new HashMap<Integer, Integer>();
		Map<Integer, Integer> rounds2 = new HashMap<Integer, Integer>();
		Map<Integer, Integer> rounds3 = new HashMap<Integer, Integer>();
		List<Integer> eachSlot1 = new ArrayList<Integer>();
		int totalSlots1 = 0;
		List<Integer> eachSlot2 = new ArrayList<Integer>();
		int totalSlots2 = 0;
		List<Integer> eachSlot3 = new ArrayList<Integer>();
		int totalSlots3 = 0;

		int slot1, slot2, slot3;
		// For every n from 2 to u calculate three values (if applicable)
		
		for (int i = 0; i < numberOfExperiments; i++) {
			slot1 = slot2 = slot3 = 0;
			
			slot1 = election_2(u, 2);
			totalSlots1 += slot1;
			eachSlot1.add(slot1);
			if (u/2 > 2) {
				slot2 = election_2(u, u/2);
				totalSlots2 += slot2;
				eachSlot2.add(slot2);
			}
			if (u > 2) {
				slot3 = election_2(u, u);
				totalSlots3 += slot3;
				eachSlot3.add(slot3);
			}
						
			if (rounds1.containsKey(slot1)) rounds1.put(slot1, rounds1.get(slot1) + 1);
			else rounds1.put(slot1, 1);
			if (slot2 != 0) {
				if (rounds2.containsKey(slot2)) rounds2.put(slot2, rounds2.get(slot2) + 1);
				else rounds2.put(slot2, 1);
			}
			if (slot3 != 0) {
				if (rounds3.containsKey(slot3)) rounds3.put(slot3, rounds3.get(slot3) + 1);
				else rounds3.put(slot3, 1);
			}
		}
		
		// Expected value and variance 1
		double expectedValue = (double) totalSlots1 / numberOfExperiments;
		double variance = 0;
		for (Integer entry: eachSlot1) {
			variance += Math.pow((double) entry - expectedValue, 2.0);
		}
		variance = variance / numberOfExperiments;
		System.out.println("Wartość oczekiwana = " + expectedValue + ", wariancja = " + variance);
		
		// Expected value and variance 2
		expectedValue = (double) totalSlots2 / numberOfExperiments;
		variance = 0;
		for (Integer entry: eachSlot2) {
			variance += Math.pow((double) entry - expectedValue, 2.0);
		}
		variance = variance / numberOfExperiments;
		System.out.println("Wartość oczekiwana = " + expectedValue + ", wariancja = " + variance);
				
		// Expected value and variance 3
		expectedValue = (double) totalSlots3 / numberOfExperiments;
		variance = 0;
		for (Integer entry: eachSlot3) {
			variance += Math.pow((double) entry - expectedValue, 2.0);
		}
		variance = variance / numberOfExperiments;
		System.out.println("Wartość oczekiwana = " + expectedValue + ", wariancja = " + variance);
		
		
		Histogram demo1 = new Histogram("Histogram - Known only u / n = 2", rounds1);
        demo1.pack();
        //RefineryUtilities.centerFrameOnScreen(demo1);
        demo1.setVisible(true);
        
        Histogram demo2 = new Histogram("Histogram - Known only u / n = u/2", rounds2);
        demo2.pack();
        //RefineryUtilities.centerFrameOnScreen(demo2);
        demo2.setVisible(true);
        
        Histogram demo3 = new Histogram("Histogram - Known only u / n = u", rounds3);
        demo3.pack();
        //RefineryUtilities.centerFrameOnScreen(demo3);
        demo3.setVisible(true);
	}
	
	public static void lemma(int u, int n, int numberOfExperiments) {
		int firstRoundsSuccess = 0;
		for (int i = 0; i < numberOfExperiments; i++) {
			int numberOfRounds = election_2_2(u, n);
			if (numberOfRounds == 1) {
				firstRoundsSuccess++;
			}
		}
		System.out.println((double) firstRoundsSuccess / numberOfExperiments);
	}	
	
	public static void lemma_2(int u, int numberOfExperiments) {
		int firstRoundsSuccess = 0;
		for (int n = 2; n <= u; n++) {
			firstRoundsSuccess = 0;
			for (int i = 0; i < numberOfExperiments; i++) {
				int numberOfRounds = election_2_2(u, n);
				if (numberOfRounds == 1) {
					firstRoundsSuccess++;
				}
			}
			System.out.println((double) firstRoundsSuccess / numberOfExperiments);

		}
		//System.out.println((double) firstRoundsSuccess / (numberOfExperiments * (u - 2)));
	}	

	public static void main(String[] args) {
		//System.out.println(election_1(1000));
		//System.out.println(election_2(100, 90));
		
		//generateHistogram_1(300, 1000);
		generateHistogram_2(1000, 1000);
		
		//lemma(1000, 900, 1000);
		//lemma_2(1000, 1000);
	}

	public static class Histogram extends ApplicationFrame {
		
		private double[][] data;
		private Map<Integer, Integer> dataMap;

		public Histogram(String title, double[][] data) {
	        super(title);
	        this.data = data;
	        IntervalXYDataset dataset = createDatasetTable();
	        JFreeChart chart = createChart(dataset);
	        final ChartPanel chartPanel = new ChartPanel(chart);
	        chartPanel.setPreferredSize(new java.awt.Dimension(500, 270));
	        setContentPane(chartPanel);
	    }
		
		public Histogram(String title, Map<Integer, Integer> dataMap) {
	        super(title);
	        this.dataMap = dataMap;
	        IntervalXYDataset dataset = createDatasetMap();
	        JFreeChart chart = createChart(dataset);
	        final ChartPanel chartPanel = new ChartPanel(chart);
	        chartPanel.setPreferredSize(new java.awt.Dimension(500, 270));
	        setContentPane(chartPanel);
	    }
	    
	    private IntervalXYDataset createDatasetTable() {
	        final XYSeries series = new XYSeries("L");
	        for (double[] pair: data) {
	        	series.add(pair[0], pair[1]);
	        }
	        final XYSeriesCollection dataset = new XYSeriesCollection(series);
	        return dataset;
	    }
	    
	    private IntervalXYDataset createDatasetMap() {
	        final XYSeries series = new XYSeries("L");
	        for (Map.Entry<Integer, Integer> entry: dataMap.entrySet()) {
	        	series.add(entry.getKey(), entry.getValue());
	        }
	        final XYSeriesCollection dataset = new XYSeriesCollection(series);
	        return dataset;
	    }

	    private JFreeChart createChart(IntervalXYDataset dataset) {
	        final JFreeChart chart = ChartFactory.createXYBarChart(
	            "Histogram",
	            "Number of rounds", 
	            false,
	            "Amount", 
	            dataset,
	            PlotOrientation.VERTICAL,
	            true,
	            true,
	            false
	        );
	        return chart;    
	    }
	}	
}