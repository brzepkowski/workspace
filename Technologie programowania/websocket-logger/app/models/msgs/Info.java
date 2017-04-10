package models.msgs;

public class Info {

    final String text;
    final String name;
    
    public String getText() {
		return text;
	}
    
    public String getName() {
		return name;
	}

	public Info(String text, String name) {
        this.text = text;
        this.name = name;
    }
}
