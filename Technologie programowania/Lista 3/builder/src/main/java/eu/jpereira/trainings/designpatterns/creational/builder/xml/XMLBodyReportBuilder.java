package eu.jpereira.trainings.designpatterns.creational.builder.xml;

import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBody;
import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBodyBuilder;

public class XMLBodyReportBuilder implements ReportBodyBuilder {

	XMLReportBody reportBody = new XMLReportBody();
	
	@Override
	public ReportBodyBuilder setCustomerName(String customerName) {
		reportBody.putContent("<sale><customer><name>");
		reportBody.putContent(customerName);
		return null;
	}

	@Override
	public ReportBodyBuilder setCustomerPhone(String phoneNumber) {
		reportBody.putContent("</name><phone>");
		reportBody.putContent(phoneNumber);
		reportBody.putContent("</phone></customer>");
		return null;
	}

	@Override
	public ReportBodyBuilder withItems() {
		reportBody.putContent("<items>");
		return null;
	}

	@Override
	public ReportBodyBuilder newItem(String name, int quantity, double price) {
		reportBody.putContent("<item><name>");
		reportBody.putContent(name);
		reportBody.putContent("</name><quantity>");
		reportBody.putContent(quantity);
		reportBody.putContent("</quantity><price>");
		reportBody.putContent(price);
		reportBody.putContent("</price></item>");
		return null;
	}

	@Override
	public ReportBodyBuilder itHasNext() {
		return null;
	}
	
	@Override
	public ReportBodyBuilder addEnding() {
		reportBody.putContent("</items></sale>");
		return null;
	}
	
	@Override
	public ReportBody getReportBody() {
		return reportBody;
	}
}