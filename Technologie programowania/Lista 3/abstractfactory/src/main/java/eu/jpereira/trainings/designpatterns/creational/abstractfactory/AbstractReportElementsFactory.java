package eu.jpereira.trainings.designpatterns.creational.abstractfactory;

public abstract class AbstractReportElementsFactory {

	protected abstract ReportBody createReportBody();
	
	protected abstract ReportFooter createReportFooter();
	
	protected abstract ReportHeader createReportHeader();
}
