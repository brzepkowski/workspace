package eu.jpereira.trainings.designpatterns.creational.builder;

import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBody;
import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBodyBuilder;

public class HTMLBodyReportBuilder implements ReportBodyBuilder {

	HTMLReportBody reportBody = new HTMLReportBody();
	
	@Override
	public ReportBodyBuilder setCustomerName(String customerName) {
		reportBody.putContent("<span class=\"customerName\">");
		reportBody.putContent(customerName);
		return null;
	}

	@Override
	public ReportBodyBuilder setCustomerPhone(String phoneNumber) {
		reportBody.putContent("</span><span class=\"customerPhone\">");
		reportBody.putContent(phoneNumber);
		reportBody.putContent("</span>");
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
		reportBody.putContent("</items>");
		return null;
	}
	
	@Override
	public ReportBody getReportBody() {
		return reportBody;
	}
}
