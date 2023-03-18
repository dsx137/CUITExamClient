#!/usr/bin/env ruby
 
require 'webrick'
require "base64"

class Post < WEBrick::HTTPServlet::AbstractServlet
  def do_POST(request, response)
    image = request.body
    # image_data = Base64.decode64(image)

    File.open("image.jpg", "wb") do |f|
    f.write(image)
    f.close
    end
    
    # set the response status and content type
    response.status = 200
    response.content_type = "text/plain"
    
    # send a message back to the client
    response.body = "Image received successfully!"
  end
end


webroot = File.expand_path ''

# create a server object
server = WEBrick::HTTPServer.new(:Port => 8000, :DocumentRoot => webroot)



# mount the Post servlet at the root path (/)
server.mount("/Img", Post)
trap 'INT' do server.shutdown end
server.start