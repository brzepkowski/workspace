require 'test_helper'

class JobOffersControllerTest < ActionController::TestCase
  setup do
    @job_offer = job_offers(:one)
  end

  test "should get index" do
    get :index
    assert_response :success
    assert_not_nil assigns(:job_offers)
  end

  test "should get new" do
    get :new
    assert_response :success
  end

  test "should create job_offer" do
    assert_difference('JobOffer.count') do
      post :create, job_offer: { city: @job_offer.city, company: @job_offer.company, descritpion: @job_offer.descritpion, name: @job_offer.name }
    end

    assert_redirected_to job_offer_path(assigns(:job_offer))
  end

  test "should show job_offer" do
    get :show, id: @job_offer
    assert_response :success
  end

  test "should get edit" do
    get :edit, id: @job_offer
    assert_response :success
  end

  test "should update job_offer" do
    patch :update, id: @job_offer, job_offer: { city: @job_offer.city, company: @job_offer.company, descritpion: @job_offer.descritpion, name: @job_offer.name }
    assert_redirected_to job_offer_path(assigns(:job_offer))
  end

  test "should destroy job_offer" do
    assert_difference('JobOffer.count', -1) do
      delete :destroy, id: @job_offer
    end

    assert_redirected_to job_offers_path
  end
end
