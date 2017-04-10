package eu.jpereira.trainings.designpatterns.creational.builder.model;

import eu.jpereira.trainings.designpatterns.creational.builder.model.ReportBody;

public interface ReportBodyBuilder {
	ReportBodyBuilder setCustomerName(String customerName);
	ReportBodyBuilder setCustomerPhone(String phoneNumber);
	ReportBodyBuilder withItems();
	ReportBodyBuilder newItem(String name, int quantity, double price);
	ReportBodyBuilder itHasNext();
	ReportBodyBuilder addEnding();
	ReportBody getReportBody();
}
