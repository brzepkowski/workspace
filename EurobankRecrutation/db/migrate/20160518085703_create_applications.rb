class CreateApplications < ActiveRecord::Migration
  def change
    create_table :applications do |t|
      t.integer :job_offer_id
      t.string :email
      t.string :name
      t.string :surname
      t.string :country
      t.string :city
      t.decimal :phone

      t.timestamps null: false
    end
		add_index :applications, :job_offer_id

  end
end
