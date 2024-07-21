from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns
import numpy as np


def exp0(fig_location):
    header = ['year','weed','direction']
    df = pd.read_csv('exp0_result.txt',sep=";",header=None,names=header)
    df['direction'] = df['direction'].fillna('none')

    fig, axes = plt.subplots(1, 1, figsize=(10, 10), sharey=False, sharex=True,)

    sns.scatterplot(x='year',y='weed',style='direction',data=df,s=100,color='black')
    sns.lineplot(x='year',y='weed',data=df,color='black',units="direction",estimator=None)


    axes.set_title("Control of Spartina alterniflora based on the starting position of removal activities")
    axes.set_ylabel("Area of Spartina alterniflora [m$^2$]")
    axes.set_xlabel("Years")
    handles, labels = plt.gca().get_legend_handles_labels()
    custom_labels = ["No intervention", "Remote areas first", "Dense growth first"]
    plt.legend(handles, custom_labels, title='Approach')

    
    axes.set_ylim(0)
    axes.set_xlim(0)

    plt.show()
    fig.savefig(fig_location)
    plt.close()


def exp1(fig_location):
    header = ['direction','work_amount','time']
    df = pd.read_csv('exp1_result.txt',sep=";",header=None,names=header)

    fig, axes = plt.subplots(1, 1, figsize=(10, 10), sharey=False, sharex=True,)

    #sns.scatterplot(x='work_amount',y='time',style='direction',data=df,color='black')
    sns.pointplot(x='work_amount',y='time',data=df,hue='direction',errorbar=lambda x:(x.min(),x.max()))

    handles, labels = plt.gca().get_legend_handles_labels()
    custom_labels = ["Remote areas first", "Dense growth first"]
    plt.legend(handles, custom_labels, title='Custom Legend')

    axes.set_title("Fight against Spartina alterniflora spread based on the starting position of removal activities")
    axes.set_ylabel("Area of Spartina alterniflora [m$^2$]")
    axes.set_xlabel("Time in years")
    axes.legend(handles, custom_labels, title='Approach')

    axes.set_ylim(0)

    plt.show()
    fig.savefig(fig_location)
    plt.close()

def exp2(fig_location):
    header = ['graph','suppresion','year','weed']
    df = pd.read_csv('exp2_result.txt',sep=";",header=None,names=header)
    print(df)

    fig, axes = plt.subplots(3, 1, figsize=(6, 12), sharey=False, sharex=True,)


    for i in [0,1,2]:
        df_specific = df.query(f'graph == -1 | graph == {10+5*i}').drop_duplicates(subset=['suppresion', 'year',])
        sns.scatterplot(ax=axes[i],x='year',y='weed',style='suppresion',data=df_specific,s=50,color='black')

        axes[i].set_title(f"Seed Spread Suppression After {10 + i * 5} Years")
        axes[i].set_ylabel("Area of Spartina alterniflora [m$^2$]")
        axes[i].set_xlabel("Time in Years")
        axes[i].set_ylim(0)
        axes[i].set_xlim(0)
        axes[i].legend().remove()

    handles, labels = plt.gca().get_legend_handles_labels()
    custom_labels = ["0,0","0,5","1,0"]
    fig.legend(handles, custom_labels, title='Seed Spread Suppression Rate', loc="outside right")
    
    fig.tight_layout()
    fig.subplots_adjust(right=0.7)


    plt.show()
    fig.savefig(fig_location)
    plt.close()


#exp0('exp0.png')
#exp1('exp1.png')
exp2('exp2.png')