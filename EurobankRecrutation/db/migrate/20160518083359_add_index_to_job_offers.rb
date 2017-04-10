class AddIndexToJobOffers < ActiveRecord::Migration
  def change
		add_index :applications, :email
  end
end
