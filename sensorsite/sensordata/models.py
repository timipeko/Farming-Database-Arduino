from django.db import models

class Measurement(models.Model):
    sensor_id = models.IntegerField()
    date = models.DateTimeField()
    temp = models.FloatField()
    humidity = models.FloatField()
    status_code = models.IntegerField()
