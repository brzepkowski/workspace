json.array!(@applications) do |application|
  json.extract! application, :id, :email, :name, :surname, :country, :region, :city, :phone
  json.url application_url(application, format: :json)
end
