/**
 * Copyright 2011 Joao Miguel Pereira
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
package eu.jpereira.trainings.designpatterns.creational.abstractfactory;

import org.junit.Test;

import eu.jpereira.trainings.designpatterns.creational.abstractfactory.Report;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.json.JSONReportElementsFactory;
import eu.jpereira.trainings.designpatterns.creational.abstractfactory.xml.XMLReportElementsFactory;

import static org.junit.Assert.*;

/**
 * @author jpereira
 * 
 */

public class ReportTest {

	@Test
	public void testCreateJSONReport() {

		AbstractReportElementsFactory factory = new JSONReportElementsFactory();
		Report report = new Report(factory);
		assertEquals("JSON", report.getBody().getType());
		assertEquals("JSON", report.getHeader().getType());
		assertEquals("JSON", report.getFooter().getType());

	}

	@Test
	public void testCreateXMLReport() {

		AbstractReportElementsFactory factory = new XMLReportElementsFactory();
		Report report = new Report(factory);
		assertEquals("XML", report.getBody().getType());
		assertEquals("XML", report.getHeader().getType());
		assertEquals("XML", report.getFooter().getType());

	}

}
