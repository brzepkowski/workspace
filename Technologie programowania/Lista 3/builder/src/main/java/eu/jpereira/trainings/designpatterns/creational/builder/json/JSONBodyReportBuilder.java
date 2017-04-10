package eu.jpereira.trainings.designpatterns.creational.builder.json;

import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBody;
import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBodyBuilder;

public class JSONBodyReportBuilder implements ReportBodyBuilder {

	JSONReportBody reportBody = new JSONReportBody();
	
	@Override
	public ReportBodyBuilder setCustomerName(String customerName) {
		reportBody.addContent("sale:{customer:{");
		reportBody.addContent("name:\"");
		reportBody.addContent(customerName);
		return null;
	}

	@Override
	public ReportBodyBuilder setCustomerPhone(String phoneNumber) {
		reportBody.addContent("\",phone:\"");
		reportBody.addContent(phoneNumber);
		reportBody.addContent("\"}");
		return null;
	}

	@Override
	public ReportBodyBuilder withItems() {
		reportBody.addContent(",items:[");
		return null;
	}

	@Override
	public ReportBodyBuilder newItem(String name, int quantity, double price) {
		reportBody.addContent("{name:\"");
		reportBody.addContent(name);
		reportBody.addContent("\",quantity:");
		reportBody.addContent(String.valueOf(quantity));
		reportBody.addContent(",price:");
		reportBody.addContent(String.valueOf(price));
		reportBody.addContent("}");
		return null;
	}

	@Override
	public ReportBodyBuilder itHasNext() {
		reportBody.addContent(",");
		return null;
	}
	
	@Override
	public ReportBodyBuilder addEnding() {
		reportBody.addContent("]}");
		return null;
	}
	
	@Override
	public ReportBody getReportBody() {
		return reportBody;
	}
}
