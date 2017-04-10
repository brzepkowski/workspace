package models.server;

/**Wyjatki uzywane przez serwer
 * @author Kornel Mirkowski
 */
@SuppressWarnings("serial")
public class BadugiException extends Exception {
	/**Konstruktor
	 * @param arg0 Tresc komunikatu wyjatku
	 */
	public BadugiException(String arg0) {
		super(arg0);
	}
}
