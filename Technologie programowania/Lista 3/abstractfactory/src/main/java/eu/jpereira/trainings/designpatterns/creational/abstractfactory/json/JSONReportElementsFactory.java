package eu.jpereira.trainings.designpatterns.creational.abstractfactory.json;

import eu.jpereira.trainings.designpatterns.creational.abstractfactory.AbstractReportElementsFactory;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportBody;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportFooter;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportHeader;


public class JSONReportElementsFactory extends AbstractReportElementsFactory {

	@Override
	protected ReportBody createReportBody() {
		return new JSONReportBody();
	}

	@Override
	protected ReportFooter createReportFooter() {
		return new JSONReportFooter();
	}

	@Override
	protected ReportHeader createReportHeader() {
		return new JSONReportHeader();
	}

}
