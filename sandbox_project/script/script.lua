Chopper = {}

Chopper.onCreate = function()
   print("OnCreate")
end

Chopper.onUpdate = function(game_object, delta_time, ellapsed_time)
  -- local x =  ellapsed_time * 0.09
  local y = 200 + (math.cos(ellapsed_time * 0.001) * 50)
  set_position(game_object, x, y)
end

Chopper.onDestroy = function()
   print("OnDestroy")
end
