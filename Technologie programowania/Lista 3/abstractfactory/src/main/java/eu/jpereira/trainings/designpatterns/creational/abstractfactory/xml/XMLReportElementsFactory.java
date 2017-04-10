package eu.jpereira.trainings.designpatterns.creational.abstractfactory.xml;

import eu.jpereira.trainings.designpatterns.creational.abstractfactory.AbstractReportElementsFactory;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportBody;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportFooter;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.ReportHeader;

public class XMLReportElementsFactory extends AbstractReportElementsFactory {

	@Override
	protected ReportBody createReportBody() {
		return new XMLReportBody();
	}

	@Override
	protected ReportFooter createReportFooter() {
		return new XMLReportFooter();
	}

	@Override
	protected ReportHeader createReportHeader() {
		return new XMLReportHeader();
	}

}
