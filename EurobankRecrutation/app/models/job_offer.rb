class JobOffer < ActiveRecord::Base
	has_many :applications, dependent: :destroy
	accepts_nested_attributes_for :applications
end
