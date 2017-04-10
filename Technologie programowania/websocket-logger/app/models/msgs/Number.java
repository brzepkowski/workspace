package models.msgs;

public class Number {

    final int nr;
    final String name;

    
    public int getNumber() {
		return nr;
	}
    
    public String getName() {
		return name;
	}

	public Number(int nr, String name) {
        this.nr = nr;
        this.name = name;

    }

}
