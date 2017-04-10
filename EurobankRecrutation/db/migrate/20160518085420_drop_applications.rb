class DropApplications < ActiveRecord::Migration
  def up
		drop_table :applications
  end
end
