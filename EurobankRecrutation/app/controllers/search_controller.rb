class SearchController < ApplicationController
  # Prevent CSRF attacks by raising an exception.
  # For APIs, you may want to use :null_session instead.
  #protect_from_forgery with: :exception
	protect_from_forgery with: :null_session

	# GET /search
	def index
		@keywords = params.require(:keywords)
		@single_keywords = @keywords.split()
		
		@search_results = Array.new
		@single_keywords.each do |k|
			@search_results << JobOffer.where("name like ? OR position like ? OR city like ? OR specification like ? OR requirements like ? OR salary like ? OR start_date like ? OR end_date like ?", "%#{k}%", "%#{k}%", "%#{k}%", "%#{k}%", "%#{k}%", "%#{k}%", "%#{k}%", "%#{k}%")
		end
	end

	# GET /search/1
	def show
	end

	# POST /search
	def create
		@keywords = params.require(:keywords)
		redirect_to action: "index", keywords: @keywords
	end
end
