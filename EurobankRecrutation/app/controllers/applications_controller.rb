class ApplicationsController < ApplicationController
  before_action :set_application, only: [:show, :edit, :update, :destroy]
	before_filter :get_job_offer


	def invite  
		@application = @job_offer.applications.find(params[:id])
		
 	  ApplicantsMailer.invite(@application).deliver  
  	redirect_to [@job_offer, @application], notice: 'Wiadomość została wysłana.' 
	end  


	def get_job_offer
		@job_offer = JobOffer.find(params[:job_offer_id])
	end


  # GET /applications
  # GET /applications.json
  def index
    @applications = @job_offer.applications
  end

  # GET /applications/1
  # GET /applications/1.json
  def show
		 @applications = @job_offer.applications.find(params[:id])
  end

  # GET /applications/new
  def new
    @application = Application.new
  end

  # GET /applications/1/edit
  def edit
  end

  # POST /applications
  # POST /applications.json
  def create
    @application = @job_offer.applications.new(application_params)

    respond_to do |format|
      if @application.save
        format.html { redirect_to [@job_offer, @application], notice: 'Application was successfully created.' }
        format.json { render :show, status: :created, location: @application }
      else
        format.html { render :new }
        format.json { render json: @application.errors, status: :unprocessable_entity }
      end
    end
  end

  # PATCH/PUT /applications/1
  # PATCH/PUT /applications/1.json
  def update
    respond_to do |format|
      if @application.update(application_params)
        format.html { redirect_to job_offer_application_path(@application.job_offer, @application), notice: 'Application was successfully updated.' }
        format.json { render :show, status: :ok, location: @application }
      else
        format.html { render :edit }
        format.json { render json: @application.errors, status: :unprocessable_entity }
      end
    end
  end

  # DELETE /applications/1
  # DELETE /applications/1.json
  def destroy
    @application.destroy
    respond_to do |format|
      format.html { redirect_to applications_url, notice: 'Application was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private
    # Use callbacks to share common setup or constraints between actions.
    def set_application
      @application = Application.find(params[:id])
    end

    # Never trust parameters from the scary internet, only allow the white list through.
    def application_params
      params.require(:application).permit(:email, :name, :surname, :country, :region, :city, :phone)
    end
end
