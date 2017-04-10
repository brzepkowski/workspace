json.array!(@job_offers) do |job_offer|
  json.extract! job_offer, :id, :name, :company, :city, :descritpion
  json.url job_offer_url(job_offer, format: :json)
end
